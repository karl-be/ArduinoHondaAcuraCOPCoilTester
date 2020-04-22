// Arduino Based PWM Control for 3 Wire or 4 Wire COP Coil Triggering And Testing
// Useful for triggering and testing 3 or 4 wire honda COP triggering spark plugs or any low current 'trigger' based coil.
//
//
//                             Spark Coil
//                           TRG   GND    12V
//                            |     |      |
//    Arduino                 |     |      |
//        ->P10 - - - - - - - -     |      |
//                                  |      |
//        ->GND - - -               |      |
//                   |              |      |
//    12V Source     |              |      |
//                   |              ^      |
//       ->12V - - -^|^- - - - - - - - - - -
//                   |              ^
//       ->GND - - - - - - - - - - -|
//
// Notes: 
//            |       
//      A - -^|^- -   Indicates that the wire from L to R does *not* connect.
//            |
//
//      This is for TRIGGERED coils that want 3 -> 5 volts on the trigger (Some triggers want to be pulled to ground see: https://github.com/karl-be/ArduinoEFIInjectorTest/blob/master/InjectorTest.ino for using a transistor / mosfet to do this).
//        if your coil is "always on" while running this it will melt in short order. Check the trigger type manually first! 
//
//      Ensure the 12V source can supply several amps of current 1-3 depending on coil type and pulse frequency and dwell. 
//      
//      Some COP Coils have 4 wires that wire returning to the ECU can be safely ignored (do not short it though).
//
//      If using a benchtop power supply ensure that the grounds are all equal between the Arduino power and the 12V source. Using a battery would work as well. 
//
//      Coils can overheat if opened too long or with too long of a dwell (duty cycle) keep testing parameters within the engines RPM and spark limitations.
//
//      Your creating a small ark, dont touch it, dont start a fire :) Have fun.
//
// Configuration: 
//
// Target RPM 
const int targetRPM = 7000;
// Enables A Wasted Spark (EG A Spark Every Rotation Of The Crankshaft, Once On Compression, One On Exhaust on a 4 stroke)
const bool enableWastedSpark = true;
// Target spark dwell time in milliseconds (usually between 2 and 5 ms) cannot exceed 100% duty cycle it will also melt coils doing this.
const int sparkDwellTimeMS = 3;
// PWM Pin 
const int triggerPin = 10;
// 
// Time after initial setup time to wait before triggering the first spark pulse (default 5 seconds).
const int waitTimeBeforeStartingRunMS = 5000;
// The total time to run the injector cycle (default 20 seconds)
const int totalRunTimeInMS = 20000;


// Internal Timing Variables Do Not Configure, They are calculated in setup / between loops.

// time to sleep between pulses
int sleepTimeMS = -1;
// keeps track of how long we have been running
int runTimeCounter = 0;


void setup() {
  // Calculate Timing (targetRPM / 60) to get revolutions per second / 1000 to get time in milliseconds between revolutions * 2 for 4 cycle engines 
  int pulseTimeMS = (1000 / (targetRPM / 60));
  if(!enableWastedSpark) {
    pulseTimeMS = pulseTimeMS * 2;
  }
  // Calculate Time To Sleep Before Opening Injector (Cannot be negative (eg > 100% duty cycle)).
  sleepTimeMS = pulseTimeMS - sparkDwellTimeMS;

  // Set Output Pins (PWM and DEBUG LED)
  pinMode(triggerPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Check if sleep time is negative and flash on board LED on / off every 1 second do not power the injector.
  // Please ensure injectorOpenTime is not greater than pulseTime.
  while (sleepTimeMS < 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  
  // Wait Before Triggering The Main Loop, Ensure Fuel Pump Is Running ECT.
  delay(waitTimeBeforeStartingRunMS);
}

void loop() {
  if(runTimeCounter < totalRunTimeInMS)
  {
    // Wait for initial delay, trigger high, wait for open time, trigger low, calculate total time to run counter.
    delay(sleepTimeMS);
    digitalWrite(triggerPin, HIGH);digitalWrite(LED_BUILTIN, HIGH);
    delay(sparkDwellTimeMS);
    digitalWrite(triggerPin, LOW);digitalWrite(LED_BUILTIN, LOW);

    runTimeCounter = runTimeCounter + sleepTimeMS + sparkDwellTimeMS;
  }
}


