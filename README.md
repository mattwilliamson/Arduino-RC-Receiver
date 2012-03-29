This is a simple project which will control a Radio Controlled vehicle via a serial interface, such as XBee or Bluetooth. The included `joystick.py` will use a joystick to transmit via XBee (or whatever serial interface you are using).

## Requirements:

* pyserial
* pygame

## Usage

* Upload the `serial_ppm_rx` sketch to your Arduino.
* Connect your servos to your Arduino per the below pinouts
* Run `python joystick.py`
* Use your joystick and watch the servos go!

I am using an XBee hooked up to an arduino plugged into my computer as the serial out interface and the other XBee is connected to my Arduino.

You can use whatever radio (including bluetooth) as long as your computer recognizes it as a serial port and the receiver spits out TTL on RX0 on your Arduino.

One example would be a standard bluetooth adapter on your computer and one of these [bluetooth ttl adapters](http://www.amazon.com/gp/product/B006T0Q8AQ/ref=as_li_ss_tl?ie=UTF8&tag=appdelinc-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=B006T0Q8AQ).

As for the joystick, I am using a [Saitek ST290](http://www.amazon.com/gp/product/B0000AW9RE/ref=as_li_ss_tl?ie=UTF8&tag=appdelinc-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=B0000AW9RE)
so you may need to change the code a little to match the buttons you want to perform which function.

Currently all buttons work as toggles.


## Caveats

* Set your XBees to 38400 baud. This was picked since it's safe to use with Series 1 XBee radios.


## Pinouts

This is as it relates to the `joystick.py` control file and it maps to the Saitek joystick mentioned above.

* Arduino Digital 2: Throttle
* Arduino Digital 3: Roll
* Arduino Digital 4: Pitch
* Arduino Digital 5: Yaw
* Arduino Digital 6: Aux1
* Arduino Digital 7: Aux2
* Arduino Digital 8: Aux3
* Arduino Digital 9: Aux4
* Arduino Digital 13: Status LED

![Schematic](https://github.com/mattwilliamson/Arduino-RC-Receiver/raw/master/serial_ppm_rx_bb.png)


![Home Made Receiver Board](https://github.com/mattwilliamson/Arduino-RC-Receiver/raw/master/receiver_board.png)
