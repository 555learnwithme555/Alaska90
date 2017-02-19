/*******
3D Printer Driver Firmware for Melzi board, used as a sub-Melzi
driven by the main Melzi.

Input: Pin A1 is stepper step line.


Dr. Orion Lawlor, lawlor@alaska.edu, 2017-02-13 (public domain)
*/

// Constants from http://reprap.org/wiki/Melzi
#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_MIN_PIN          18

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_MIN_PIN          19

#define Z_STEP_PIN         3
#define Z_DIR_PIN          2
#define Z_MIN_PIN          20

#define E0_STEP_PIN         1
#define E0_DIR_PIN          0

#define LED_PIN            27

#define FAN_PIN            4 

#define HEATER_0_PIN       13 // extruder

#define HEATER_BED_PIN     10 // bed (change to 12 for breakout pin on header)
#define X_ENABLE_PIN       14
#define Y_ENABLE_PIN       14
#define Z_ENABLE_PIN       26
#define E0_ENABLE_PIN      14
#define ENABLE_OFF 1
#define ENABLE_ON 0 /* active low, for some reason */

#define TEMP_0_PIN          7   /* Analogue pin */
#define TEMP_BED_PIN        6   /* Analogue pin */
#define SDSS               31


#define HAMMER_PIN FAN_PIN



long last_millis=-10000; // main control loop counter
long last_step_time=-10000; // time we last saw a stepper command
int last_step_val=1;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Sub-Melzi firmware (lawlor@alaska.edu, 2017-02-14) starting. (kHz)\n");
  pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,1);
  
  // Turn off everything
  pinMode(X_ENABLE_PIN,OUTPUT); digitalWrite(X_ENABLE_PIN,ENABLE_OFF);
  pinMode(Y_ENABLE_PIN,OUTPUT); digitalWrite(Y_ENABLE_PIN,ENABLE_OFF);
  pinMode(Z_ENABLE_PIN,OUTPUT); digitalWrite(Z_ENABLE_PIN,ENABLE_OFF);
  pinMode(HEATER_BED_PIN,OUTPUT); digitalWrite(HEATER_BED_PIN,0);
  
  // Turn on extruder stepper driver
  pinMode(E0_ENABLE_PIN,OUTPUT); digitalWrite(E0_ENABLE_PIN,ENABLE_OFF); // steppers off until commanded
  pinMode(E0_DIR_PIN,OUTPUT); digitalWrite(E0_DIR_PIN,1); // doublecheck direction
  pinMode(E0_STEP_PIN,OUTPUT);
  
  // Hammer solenoid output
  pinMode(HAMMER_PIN,OUTPUT); digitalWrite(HAMMER_PIN,0);
  
  // Input from main Melzi (and pullup to avoid noise)
  pinMode(A1,INPUT_PULLUP);
}

  long iter=0;
void loop(void) {
  long cur; // current time, in milliseconds (ish, skips every 43ms)
  while (last_millis==(cur=millis())) 
  { // forward stepper commands here: this loop runs at about 130kHz
    int step=digitalRead(A1);
    if (last_step_val!=step) 
    { // stepper moved
      last_step_time=cur;
      last_step_val=step;
      digitalWrite(E0_ENABLE_PIN,ENABLE_ON); // steppers on
      digitalWrite(E0_STEP_PIN,step); // forward the step command
      iter++;
    }
  }
  last_millis=cur;
  
  // Hammer logic (every 1ms is plenty)
  bool hammertime=0;
  if (cur-last_step_time<100) 
  { // run tamping hammer
    hammertime=1;
    int hammer=(cur%60<40); // smack period and duty cycle
    digitalWrite(FAN_PIN,hammer);
    digitalWrite(LED_PIN,hammer); // fast blink to indicate tamping active
  } else { // now stop
    digitalWrite(FAN_PIN,0); // hammer off
    digitalWrite(E0_ENABLE_PIN,ENABLE_OFF); // stepper off
  }
  
  // Debug printouts: kHz rate, minus if tamping.
  if (cur%512==0) {
    if (hammertime) iter=-iter;
    Serial.println(iter);
    iter=0;
  }
  
  // Alive indicator blinking.
  int blink=(cur%2048)<100;
  digitalWrite(LED_PIN,blink);
}


