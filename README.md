# 8266_led_ghost. 
This project creates an LED "ghosting" images.  I've created a daughter board with 8 LEDs (controlled via a shift-register)
that will light up in sequence to spell out a message when moved.  My first prototype is for a frisbee, but this could also 
used on a bike tire, or anthing that spins or travels fairly quickly.

## LED control
I'm using a standard 74595 shift register to give 8 outputs for the LEDs with only 3 inputs.  For each row of pixels, the code
shifts in those 8 bits and then latches them, which causes the LEDs to display.  

In order to do characters, I've got maps of each character...a "1" indicates that pixel is "on", and a "0" indicates off.
Each character contains 6 rows of 8 pixels; the code runs through those pixels to do the virtual display.

On top of that, the code stores a string for the message display.  We iterate over that string, character-by-character to
get the whole message.  

## Web control
The "ghost" hosts it's own web page where you can set the display string and other parameters.  
The 8266 can either operate in Station-mode (where it connects to an existing WiFi network) or AP mode (where it creates it's
own WiFi network.  I've chosen the latter...that way you can still change the string if you are out of WiFi coverage.

To do the web control, I'm using Arduino's ESP8266WiFi.h and ESP8266WebServer.h libraries.  The web server library lets you set 
up handler functions for when a client connects, and also how to process inputs.  It's been a while since I've coded HTML and CSS; 
I used these links to help:
https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Your_first_HTML_form
https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
