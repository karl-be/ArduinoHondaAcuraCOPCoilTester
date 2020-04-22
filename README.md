## Arduino Based PWM Control for 3 Wire or 4 Wire COP Coil Triggering And Testing

Useful for triggering and testing 3 or 4 wire honda COP triggering spark plugs or any low current 'trigger' based coil.


                             Spark Coil
                           TRG   GND    12V
                            |     |      |
    Arduino                 |     |      |
        ->P10 - - - - - - - -     |      |
                                  |      |
        ->GND - - -               |      |
                   |              |      |
    12V Source     |              |      |
                   |              ^      |
       ->12V - - -^|^- - - - - - - - - - -
                   |              ^
       ->GND - - - - - - - - - - -|
    
    Notes: 
              |       
        A - -^|^- -   Indicates that the wire from L to R does *not* connect.
              |

      This is for TRIGGERED coils that want 3 -> 5 volts on the trigger (Some triggers want to be pulled to ground see: https:github.com/karl-be/ArduinoEFIInjectorTest/blob/master/InjectorTest.ino for using a transistor / mosfet to do this).
        if your coil is "always on" while running this it will melt in short order. Check the trigger type manually first! 

      Ensure the 12V source can supply several amps of current 1-3 depending on coil type and pulse frequency and dwell. 
      
      Some COP Coils have 4 wires that wire returning to the ECU can be safely ignored (do not short it though).

      If using a benchtop power supply ensure that the grounds are all equal between the Arduino power and the 12V source. Using a battery would work as well. 

      Coils can overheat if opened too long or with too long of a dwell (duty cycle) keep testing parameters within the engines RPM and spark limitations.

      Your creating a small ark, dont touch it, dont start a fire :) Have fun.
