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


// I/O for sandstruder stepper
#define STRUDER_IN_PIN A1
#define STRUDER_ENABLE_PIN E0_ENABLE_PIN
#define STRUDER_STEP_PIN E0_STEP_PIN
#define STRUDER_DIR_PIN E0_DIR_PIN

// I/O for solenoid tamper:
#define TAMPER_IN_PIN A2
#define TAMPER_OUT_PIN FAN_PIN



long last_millis=-10000; // main control loop counter
long last_step_time=-10000; // time we last saw a stepper command
int last_step_val=1;
long last_tamp_time=-10000; // time we last saw a tamper command
int last_tamp_val=1;

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
  pinMode(STRUDER_ENABLE_PIN,OUTPUT); digitalWrite(STRUDER_ENABLE_PIN,ENABLE_OFF); // steppers off until commanded
  pinMode(STRUDER_DIR_PIN,OUTPUT); digitalWrite(STRUDER_DIR_PIN,1); // doublecheck direction
  pinMode(STRUDER_STEP_PIN,OUTPUT);
  
  // Hammer solenoid output
  pinMode(TAMPER_OUT_PIN,OUTPUT); digitalWrite(TAMPER_OUT_PIN,0);
  
  // Input from main Melzi (and pullup to avoid noise)
  pinMode(STRUDER_IN_PIN,INPUT_PULLUP);
  pinMode(TAMPER_IN_PIN,INPUT_PULLUP);
}

void loop(void) {
  unsigned int n_iter=0, n_step=0, n_tamp=0;
  long cur; // current time, in milliseconds (ish, skips every 43ms)
  while (last_millis==(cur=millis())) 
  { // forward stepper commands here: this loop runs at about 130kHz
    n_iter++;
    int step=digitalRead(STRUDER_IN_PIN);
    if (last_step_val!=step) 
    { // stepper moved
      n_step++;
      last_step_val=step;
      digitalWrite(STRUDER_ENABLE_PIN,ENABLE_ON); // steppers on
      digitalWrite(STRUDER_STEP_PIN,step); // forward the step command
    }
    int tamp=digitalRead(TAMPER_IN_PIN);
    if (last_tamp_val!=tamp)
    { // tamper moved
      n_tamp++;
      last_tamp_val=tamp;
    }
  }
  last_millis=cur;
  
  if (n_tamp>2) last_tamp_time=cur; // ignore short tamper blips?
  if (n_step>0) last_step_time=cur;

  // Stepper enable logic
  if (cur-last_step_time<10)
  {
    // digitalWrite(LED_PIN,1); // solid on to indicate stepper active
  } else { // stop
    digitalWrite(STRUDER_ENABLE_PIN,ENABLE_OFF); // stepper off (avoid heating)
  // HACK: only tamp if stepper is disabled.  
  //  needed because some Marlin bug randomly whacks the hammer while printing.

    // Tamp logic (every 1ms is plenty)
    bool hammertime=0;
    if (cur-last_tamp_time<10) 
    { // run tamping hammer
      hammertime=1;
      int period=64; // tamping cycle time in ms
      int on_time=7; // duty cycle: ms of on time during each cycle. 6 is min at 20v bus.
      int tamper=(cur%period <= on_time); // timing based
      digitalWrite(TAMPER_OUT_PIN,tamper);
      digitalWrite(LED_PIN,tamper); // fast blink to indicate tamping active
    } else { // now stop
      digitalWrite(TAMPER_OUT_PIN,0); // tamper off
    }
  }
  
  // Debug printouts: kHz rate, minus if tamping.
  if (cur%512==0) {
    // if (hammertime) iter=-iter;
    Serial.println(n_iter);
  }
  
  // Alive indicator blinking.
  int blink=(cur%2048)<100;
  digitalWrite(LED_PIN,blink);
}


