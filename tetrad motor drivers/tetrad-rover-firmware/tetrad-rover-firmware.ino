//---motor stuff
#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100

/* RGB Pins */
#define R_PIN 20
#define G_PIN 22
#define B_PIN 21

/*
  pin definitions:
  xxx[0] controls '1' outputs
  xxx[1] controls '2' outputs */
int inApin[2] = {27, 30};  // INA: Clockwise input
int inBpin[2] = {28, 29}; // INB: Counter-clockwise input
int pwmpin[2] = {25, 32}; // PWM input
int cspin[2] = {31, 26}; // CS: Current sense ANALOG input
int enpin[2] = {24, 33}; // EN: Status of switches output (Analog pin)

//---status
const int statpin = 13;

byte address = 3 + 128; //address of this motor controller

int len = 4;

byte command_buf[5]; //create string to hold command received

byte num_bytes;

void setup(){
	Serial2.begin(38400);
	Serial.begin(115200);
	pinMode(statpin, OUTPUT);
	pinMode(R_PIN, OUTPUT);
	pinMode(G_PIN, OUTPUT);
	pinMode(B_PIN, OUTPUT);
	digitalWrite(R_PIN, HIGH);
	digitalWrite(G_PIN, HIGH);
	digitalWrite(B_PIN, HIGH);
	// Initialize digital pins as outputs
	for (int i = 0; i < 2; i++){
		pinMode(inApin[i], OUTPUT);
		pinMode(inBpin[i], OUTPUT);
		pinMode(pwmpin[i], OUTPUT);
	}
	// Initialize braked
	for (int i = 0; i < 2; i++){
		digitalWrite(inApin[i], LOW);
		digitalWrite(inBpin[i], LOW);
	}
	//set PWM frequency
	analogWriteFrequency(pwmpin[0], 15000);
	analogWriteFrequency(pwmpin[1], 15000);
}

uint8_t count = 0;

void loop() {
	if(Serial2.available()){
		byte b = Serial2.read();
		//digitalWrite(B_PIN, !digitalRead(B_PIN));
		if(b >= 128){
			/* This is a command byte. */
			count = 0;
		}
		command_buf[count] = b;
		count++;
	}

	if (count == 5) { //if we have a command_buf we need to parse and update things
		Serial.print("Got packet. Address = ");
		Serial.println(command_buf[0]);
		if (((command_buf[0] + command_buf[1] + command_buf[2] + command_buf[3]) & 127) == command_buf[4]) { //checksum is good
			if (command_buf[0] == address) { //its our address
				Serial.println("Got packet for this driver.");
				set_motor_output(command_buf[1], command_buf[2], command_buf[3]);
			}
		}
		count = 0; //reset command_buf to zero
	}
}

void set_motor_output(uint8_t dir, uint8_t m1, uint8_t m2){	
	if(m1 == 0){
		/* Brake to ground */
		digitalWrite(inApin[0], HIGH);
		digitalWrite(inBpin[0], HIGH);
		digitalWrite(G_PIN, HIGH);
	} else {
		digitalWrite(G_PIN, LOW);
		digitalWrite(inApin[0], !!(dir & _BV(1)));
		digitalWrite(inBpin[0],  !(dir & _BV(1)));
	}
	if(m2 == 0){
		digitalWrite(R_PIN, HIGH);
		digitalWrite(inApin[1], HIGH);
		digitalWrite(inBpin[1], HIGH);
	} else {
		digitalWrite(R_PIN, LOW);
		digitalWrite(inApin[1], !!(dir & _BV(2)));
		digitalWrite(inBpin[1],  !(dir & _BV(2)));
	}
	analogWrite(pwmpin[0], m1 * 2);
	analogWrite(pwmpin[1], m2 * 2);
}
