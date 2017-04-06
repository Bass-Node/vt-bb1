# Author: Niels Huisman, BS.ELM @ VTC
# Spring 2017
# Attempting to merge both the servo control and the sonar modules
# into one big, ugly, messy chunk of code.  Not sure how to do this better
# yet, but will try to figure it out.  Right now, just need to get this working
#
# -----------------------
# Pinout for sonar:
# VCC    - Pin 2
# Trig   - Pin 16
# Echo   - Pin 18
# Gnd    - Pin 6
#
# Pinout for servo:
# VCC    - Pin 2
# Gnd    - Pin 6
# Signal - Pin 7
#
# Use a voltage divider to ensure no more than 3.3V returns to the Pi
#
# Echo--1kOhm---1kOhm-- Pin 18
#             |
#           2kOhm
#             |
# Gnd------------------ Pin 6
# 
# -----------------------

# ------------------------------
# Import required python libraries
# ------------------------------

from __future__ import print_function
import RPi.GPIO as GPIO
import time
import matplotlib.pyplot as plt

# ------------------------------
# Set up GPIO pins
# ------------------------------

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)
GPIO.setup(18, GPIO.IN)
GPIO_TRIGGER = 16
GPIO_ECHO = 18
p = GPIO.PWM(7,50) #Sets pin 7 to be PWM, with a freq of 50Hz
p.start(7.5)
angle = 7.5
goUp = 1

# ------------------------------
# Functions
# ------------------------------

def measure():
    """ A function to measure a distance"""

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
    """ Takes 3 Measurements and returns the average"""
    
    distance1=measure()
    time.sleep(0.001)
    distance2=measure()
    time.sleep(0.001)
    distance3=measure()
    distance = distance1 + distance2 + distance3
    distance = distance / 3
    return distance

# ------------------------------
# Main Loop
# ------------------------------

# Speed of sound in cm/s at temperature
temperature = 20
speedSound = 33100 + (0.6*temperature)

print("Ultrasonic Measurement")
print("Speed of sound is",speedSound/100,"m/s at ",temperature,"deg")

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
        p.ChangeDutyCycle(angle)
        distance = measure()
        print("Distance : {0:5.1f}".format(distance))
        time.sleep(.01)
        if(goUp == 1 and angle < 9):
            angle += 0.1
        elif(goUp == 1 and angle >= 9):
            goUp = 0
            angle -= 0.1
        elif(goUp == 0 and angle > 6):
            angle -= 0.1
        elif(goUp == 0 and angle <= 6):
            goUp = 1
            angle += 0.1

except KeyboardInterrupt:
    # User pressed CTRL-C
    # Reset GPIO settings
    print('Stopped')
    GPIO.cleanup()
