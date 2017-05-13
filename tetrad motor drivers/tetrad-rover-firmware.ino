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

byte address = 0 + 128; //address of this motor controller

int len = 4;

byte command_buf[4]; //create string to hold command received

byte num_bytes;

void setup(){
   Serial2.begin(9600);
   pinMode(statpin, OUTPUT);
   pinMode(R_PIN, OUTPUT);
   pinMode(G_PIN, OUTPUT);
   pinMode(B_PIN, OUTPUT);
   digitalWrite(R_PIN, HIGH);
   digitalWrite(G_PIN, HIGH);
   digitalWrite(B_PIN, HIGH);
   
  // Initialize digital pins as outputs
  for (int i = 0; i < 2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  // Initialize braked
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }

  //set PWM frequency
  analogWriteFrequency(pwmpin[0], 187500);
  analogWriteFrequency(pwmpin[1], 187500);

}

uint8_t count = 0;

void loop() {
  if(Serial2.available()){
    byte b = Serial2.read();
    //digitalWrite(B_PIN, !digitalRead(B_PIN));
    if(b > 128){
      /* This is a command byte. */
      count = 0;
    }
    command_buf[count] = b;
    count++;
  }

  if (count == 4) { //if we have a command_buf we need to parse and update things
    //check the checksum
    if (((command_buf[0] + command_buf[1] + command_buf[2]) & 127) == command_buf[3]) { //checksum is good
      if (command_buf[0] == address) { //its our address
        switch (command_buf[1]) {//update everything
          case 0: //drive motor 1 forward
            set_motor_output(0, 1, command_buf[2]);
            break;
          case 1: //drive motor 1 backward
            set_motor_output(0, 2, command_buf[2]);
            break;
          case 4: //drive motor 2 forward
            set_motor_output(1, 1, command_buf[2]);
            break;
          case 5: //drive motor 2 backwards
            set_motor_output(1, 2, command_buf[2]);
            break;
        }
      }
    }
    count = 0; //reset command_buf to zero
  }
}

void set_motor_output(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  /* Set status RGB led. */
  if(motor == 0){
    if(pwm == 0){
      digitalWrite(G_PIN, HIGH);
    } else {
      digitalWrite(G_PIN, LOW);
    }
  } else {
    if(pwm == 0){
      digitalWrite(R_PIN, HIGH);
    } else {
      digitalWrite(R_PIN, LOW);
    }
  }
  if (motor <= 1)
  {
    if (direct <= 4)
    {
      // Set inA[motor]
      if (direct <= 1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct == 0) || (direct == 2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], map(pwm, 0, 127, 0, 255));
    }
  }
}
