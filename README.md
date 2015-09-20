# Spooky Eyes
This lttile project is about having fun with an Arduino. For my daughters spooky birthday party (or Halloween if you want), I wanted to something special. Some years ago my wife bought me an iron helmet on a wooden stand. Wouldn't it be awesome to make the helmet come alive with two demonic eyes? We also bought a styrofoam skull. Now, I can make two demons ;) Here's what I did...

## Materials

These are the materials I used to create one set of eyes:

- 2x 10mm diffused red LEDs
- 2x 150 Ohm resistors
- 1x Adafruit Perma-Proto Quarter-sized Breadboard PCB
- 1x Arduino Mini
- 1x 9V Battery Adapter
- 1x 9V Battery
- Some wires

I carefully sawed the Perma-Proto breadboard into two pieces.

## Arduino code

See spooky_eyes.ino on GitHub

## Uploading code to the Arduino mini

The Arduino mini does not have a build in USB connector, so we need another way to upload code to it. I tried a PL2303TA USB to Serial Bridge Controller, but that didn't work (I heard the cable delivers 3V power while the Mini needs 5V). Everything I tried resulted in this error:

avrdude: stk500_getsync(): not in sync: resp=0x00

So, I switched to a USB serial adapter (https://www.arduino.cc/en/Main/USBSerial). Read the following page for instructions: https://www.arduino.cc/en/Guide/ArduinoMini

The first attempt gave me the same error as before, but then I removed the reset pin by accident...

Summary:

- GRN -> GRN
- PWR -> PWR
- RX -> TX
- TX -> RX
- Reset -> Reset
- !!! remove the reset pin when avrdude is trying to upload the code !!!
- done

Now the Arduino is programmed!

## Eye effects

The code I wrote can let the eyes do the following effects:

- The eyes can open (fade in);
- The eyes can close (fade out);
- When open, the eyes blink every 10-15 seconds;
- The eyes can go crazy (flicker) once in a while
