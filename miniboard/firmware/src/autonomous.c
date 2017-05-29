/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * autonomous.h - Autonomous point-to-point driving.
 * Author(s): Nick Ames
 */
#include "autonomous.h"
#include <math.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "commgen.h"

ISR(TIMER3_COMPA_vect){
	Data->time_ms++;
}

/* Return the current time value. */
static uint32_t get_ms(void){
	uint32_t temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		temp = Data->time_ms;
	}
	return temp;
}
	
/* Start the timer used for autonomous driving. */
void autonomous_init(void){
	TCCR3B = _BV(WGM32);
	TCNT3 = 0;
	OCR3A = (uint16_t) 249;
	TIMSK3 = _BV(OCIE3A);
	TCCR3B |= _BV(CS31) | _BV(CS30); /* Set 64 prescaler. */
}

typedef struct {
	float p;
	float i;
	float d;
	float integral;
	float last_error;
} pid_t;

/* Given a PID and current error, return the output of the PID controller.
 * Call this function only once per data sample, since it sets the last error value. */
static float compute_pid(pid_t *pid, float error){
	if(error > 0 && pid->last_error < 0){
		pid->integral = 0;
	}
	if(error < 0 && pid->last_error > 0){
		pid->integral = 0;
	}
	float derivative = error - pid->last_error;
	pid->integral += derivative;
	pid->last_error = error;
	return (pid->p * error) + (pid->i * pid->integral) + (pid->d * derivative);
}

/* Reset a PID. */
static void reset_pid(pid_t *pid){
	pid->integral = 0;
	pid->last_error = 0;
}

/* Clamp a float to [-1, 1], then convert it to an int8_t from -127 to 127. */
static int8_t motorf(float in){
	if(in > 1.0){
		in = 1.0;
	}
	if(in < -1.0){
		in = -1.0;
	}
	int16_t i = in * 127.0;
	if(i > 127){
		i = 127;
	}
	if(i < -127){
		i = -127;
	}
	return i;
}

typedef struct {
	 /* In 10-6 minute units. */
	int64_t lat;
	int64_t lon;
} latlon_t;

typedef struct {
	/* In meters from starting point. */
	float x; /* East-west, positive east. */
	float y; /* North-south, positive north. */
} coord_t;

/* Convert a latlong to meters (coord_t), based on the starting point. */
coord_t to_meters(latlon_t point, latlon_t start){
	coord_t result;
	result.y = ((float) point.lat - start.lat) * ((2*M_PI*EARTH_R*10e-6)/(60.0 * 360));
	result.x = ((float) point.lon - start.lon) * ((2*M_PI*EARTH_R*10e-6*cos(start.lat*(M_PI/(180*60.0*10e-6))))/(60.0 * 360));
	return result;
}

/* Add two coord_ts as vectors. */
coord_t vec2_add(coord_t a, coord_t b){
	coord_t result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

/* Subtract two coord_ts as vectors, a-b. */
coord_t vec2_sub(coord_t a, coord_t b){
	coord_t result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

/* Return the magnitude of a vector. */
float vec2_mag(coord_t a){
	return sqrt(a.x*a.x + a.y*a.y);
}

/* Return the dot product of two vectors. */
float vec2_dot(coord_t a, coord_t b){
	return a.x*b.x + a.y*b.y;
}

/* Return a vector multiplied by a scalar. */
coord_t vec2_mul(float s, coord_t v){
	coord_t result;
	result.x = v.x * s;
	result.y = v.y * s;
	return result;
}

/* Return the vector rejection of v onto basis. */
coord_t vec2_rejection(coord_t v, coord_t basis){
	return vec2_sub(v, vec2_mul(vec2_dot(v, basis)/vec2_dot(basis, basis), basis));
}

/* Return the "handedness of a vector". */
float vec2_handedness(coord_t a, coord_t b){
	float f = a.x*-b.y + a.y*b.x;
	if(f > 0){
		return 1.0;
	} else {
		return -1.0;
	}
}

/* Process autonomy. */
void autonomous(void){
	static uint32_t last_time;
	static pid_t turn_pid;
	static pid_t speed_pid;
	static pid_t cross_pid;
	static latlon_t starting_point;
	static coord_t waypoint;
	static float target_speed;
	static float integral_turn_rate; /* Actual turn rate. */
	static float integral_speed; /* Actual speed. */
	static enum {
		CAL_1TURN,
		CAL_2WAIT,
		CAL_3SAMPLE,
		CAL_4SPIN,
		CAL_5SAMPLE,
		CAL_6WAIT,
		CAL_7TURN,
		CAL_8WAIT,
		CAL_9WAIT, /* Stop and wait for settle */
		CAL_10STRAIGHT,
		CAL_11WAIT
	} calstep;
	/* Steps
	* Turn Mode
	* Wait turn
	* sample gyro (& GPS?)
	* Spin
	* Sample compass
	* Stop when elapsed
	* Wait stop (short)
	* Get heading
	* Turn to heading
	* Straight mode
	* Wait turn 
	* Go to drive */
	static enum {
		IDLE = 0,
		STOP,
		CAL_START,
		CALIBRATE,
		TURN,
		DRIVE,
		APPROACH
	} state;
	
	turn_pid.p = TURN_P;
	turn_pid.i = TURN_I;
	turn_pid.d = TURN_D;
	speed_pid.p = SPEED_P;
	speed_pid.i = SPEED_I;
	speed_pid.d = SPEED_D;
	cross_pid.p = CROSS_P;
	cross_pid.i = CROSS_I;
	cross_pid.d = CROSS_D;
	
	uint32_t time_elapsed = get_ms() - last_time;
	last_time = get_ms();
	
	latlon_t current_latlon;
	//TODO: check for GPS valid 
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		current_latlon.lat = Data->latitude;
		current_latlon.lon = Data->longitude;
	}
	coord_t current_pos = to_meters(current_latlon, starting_point);
	
	float desired_turn; /* Desired turn rate, in deg/s. */
	float desired_speed; /* Desired speed, in m/s. */
	
	if(!Data->auton_en && state != IDLE){
		state = STOP;
	}
	
	if(state == IDLE){
		reset_pid(&turn_pid);
		reset_pid(&speed_pid);
		reset_pid(&cross_pid);
		desired_turn = 0;
		desired_speed = 0;
		if(Data->auton_en){
			state = CAL_START;
		}
	} else if (state == STOP){
		Data->l_f_drive = 0;
		Data->l_m_drive = 0;
		Data->l_b_drive = 0;
		Data->r_f_drive = 0;
		Data->r_m_drive = 0;
		Data->r_b_drive = 0;
		Data->swerve_state = 0;
	} else if(state == CAL_START){
		//TODO: set waypoint, target speed & start point
		state = CALIBRATE;
	} else if(state == CALIBRATE){
		
		
		
		//TODO
		desired_turn = COMPASS_CAL_SPIN_SPEED;
	} else if(state == TURN){
		
	} else if(state == DRIVE){
		desired_turn = compute_pid(&cross_pid, vec2_handedness(current_pos, waypoint)*vec2_mag(vec2_rejection(current_pos, waypoint)));
		desired_speed = target_speed;
		//TODO: final approach/mid-course turn logic
	} else if(state == APPROACH){
		
		
	} else {
		state = STOP;
	}
	
	if(state != IDLE && state != STOP){
		float gyro; /* Measured turn rate in degrees/sec. */
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			gyro = Data->gyro_z * .061;
		}
		integral_turn_rate += compute_pid(&turn_pid, desired_turn - gyro);
		float actual_speed;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			if(Data->gps_track_valid){
				actual_speed = Data->gps_speed / 3600.0;
			} else {
				actual_speed = fabs(integral_speed) * MAX_SPEED;
			}
		}
		integral_speed += compute_pid(&speed_pid, desired_speed - actual_speed);
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			int8_t left = motorf(integral_speed - integral_turn_rate);
			int8_t right = motorf(integral_speed + integral_turn_rate);
			Data->l_f_drive = left;
			Data->l_m_drive = left;
			Data->l_b_drive = left;
			Data->r_f_drive = right;
			Data->r_m_drive = right;
			Data->r_b_drive = right;
		}
	}
}
 
