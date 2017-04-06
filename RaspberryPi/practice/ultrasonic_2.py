#!/usr/bin/python
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#|R|a|s|p|b|e|r|r|y|P|i|-|S|p|y|.|c|o|.|u|k|
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#
# ultrasonic_2.py
# Measure distance using an ultrasonic module
# in a loop.
#
# Ultrasonic related posts:
# http://www.raspberrypi-spy.co.uk/tag/ultrasonic/
#
# Author : Matt Hawkins
# Date   : 16/10/2016
# -----------------------

# -----------------------
# Import required Python libraries
# -----------------------

# -----------------------
# Pinout
# VCC  - Pin 2  (GPIO 5V)
# Trig - Pin 16 (GPIO 23)
# Echo - Pin 18 (GPIO 24)
# Gnd  - Pin 6  (GPIO GND)
#
# Use a voltage divider to ensure no more than 3.3V returns to the Pi
#
# Echo--1kOhm------- Pin 18
#             |
#           2kOhm
#             |
# Gnd--------------- Pin 6
# 
# -----------------------

from __future__ import print_function
import time
import RPi.GPIO as GPIO

# -----------------------
# Define some functions
# -----------------------
def measure():
  # This function measures a distance
  GPIO.output(GPIO_TRIGGER, True)
  # Wait 10ns
  time.sleep(0.00001)
  GPIO.output(GPIO_TRIGGER, False)
  start = time.time()
  
  while GPIO.input(GPIO_ECHO)==0:
    start = time.time()

  while GPIO.input(GPIO_ECHO)==1:
    stop = time.time()

  elapsed = stop-start
  distance = (elapsed * speedSound)/2

  return distance

def measure_average():
  # This function takes 3 measurements and
  # returns the average.

  distance1=measure()
  time.sleep(0.01)
  distance2=measure()
  time.sleep(0.01)
  distance3=measure()
  distance = distance1 + distance2 + distance3
  distance = distance / 3
  return distance

# -----------------------
# Main Script
# -----------------------

# Use BCM GPIO references
# instead of physical pin numbers
GPIO.setmode(GPIO.BCM)

# Define GPIO to use on Pi
GPIO_TRIGGER = 23
GPIO_ECHO    = 24

# Speed of sound in cm/s at temperature
temperature = 20
speedSound = 33100 + (0.6*temperature)

print("Ultrasonic Measurement")
print("Speed of sound is",speedSound/100,"m/s at ",temperature,"deg")

# Set pins as output and input
GPIO.setup(GPIO_TRIGGER,GPIO.OUT)  # Trigger
GPIO.setup(GPIO_ECHO,GPIO.IN)      # Echo

# Set trigger to False (Low)
GPIO.output(GPIO_TRIGGER, False)

# Allow module to settle
time.sleep(0.0001)

# Wrap main content in a try block so we can
# catch the user pressing CTRL-C and run the
# GPIO cleanup function. This will also prevent
# the user seeing lots of unnecessary error
# messages.
try:
  while True:
    distance = measure_average()
    print("Distance : {0:5.1f}".format(distance))
    time.sleep(.001)

except KeyboardInterrupt:
  # User pressed CTRL-C
  # Reset GPIO settings
  GPIO.cleanup()
