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

static uint32_t Time_ms;

ISR(TIMER3_COMPA_vect){
	Time_ms++;
}

/* Return the current time value. */
static uint32_t get_ms(void){
	uint32_t temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		temp = Time_ms;
	}
	return temp;
}
	
/* Start the timer used for autonomous driving. */
void autonomous_init(void){
	TCCR3B = 0;
	TCNT3 = 0;
	OCR3A = (uint16_t) 249;
	TIMSK3 = _BV(OCIE3A);
	TCCR3B = _BV(CS31) | _BV(CS30); /* Set 64 prescaler. */
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
coord_t to_meters(const latlon_t *point, const latlon_t *start){
	coord_t result;
	result.y = ((float) point->lat - start->lat) * ((2*M_PI*EARTH_R*10e-6)/(60.0 * 360));
	result.x = ((float) point->lon - start->lon) * ((2*M_PI*EARTH_R*10e-6*cos(start->lat*(M_PI/(180*60.0*10e-6))))/(60.0 * 360));
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
float vec2_mul(float s, coord_t v){
	coord_t result;
	result.x = v.x * s;
	result.y = v.y * s;
	return result;
}

/* Return the vector rejection of v onto basis. */
float vec2_rejection(coord_t v, coord_t basis);
	

/* Process autonomy. */
void autonomous(void){
	static uint32_t last_time;
	static pid_t turn_pid;
	static pid_t speed_pid;
	static latlon_t starting_point;
	static coord_t waypoint;
	static float integral_turn_rate;
	static enum {
		IDLE = 0,
		STOP,
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
	
	uint32_t time_elapsed = get_ms() - last_time;
	last_time = get_ms();
	
	latlon_t current_latlon;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		current_latlon.lat = Data->lattidude;
		current_latlon.lon = Data->longitude;
	}
	coord_t current_pos = to_meters(current_latlon, starting_point);
	
	float gyro; /* Rotation rate in degrees/sec. */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		gyro = Data->gyro_z * .061;
	}
	float desired_turn; /* Desired turn rate. */
	
	if(state == IDLE){
		
	} else if (state == STOP){
		
	} else if(state == CALIBRATE){
		
		
	} else if(state == TURN){
		
	} else if(state == DRIVE){
		
		
	} else if(state == APPROACH){
		
		
	} else {
		state = STOP;
	}
	
	if(state != IDLE && state != STOP){
		
		
	}
}
 
