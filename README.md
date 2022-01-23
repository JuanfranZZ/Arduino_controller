# Arduino_domotic_controller

## Arduino relay timer with 7LED display

To control a delay timer in a simple way we can use:
- Arduino UNO
- relay
- 7LED 1 digit display (common cathode type)
- push button
- resistance 220 ![formula](https://render.githubusercontent.com/render/math?math=\color{white}\large\Omega) (connected to the 7LED common cathode type)
- resistance 10 ![formula](https://render.githubusercontent.com/render/math?math=\color{white}\large{K\Omega}) (connected to the push button)
- wire to connect something to be controlled by the relay (lamp, heater,...)
 
![Fritzing Sketch](https://github.com/JuanfranZZ/Arduino_controller/blob/button_7LED_timer_rele/Timer_rele_7LED_bb.jpg)

### Mode of use:

- The push button set the number of 10 minutes for the countdown timer. 
- Once the timer is running, the relay allow to pass the power.
- If the button is pressed while running, additional time to timer can be added
- If 0 is set the system is turned off.
- The 7LED dot light will be blinking in order to indicate the relay is working.
