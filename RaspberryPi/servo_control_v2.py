# -*- coding: utf-8 -*-
# Author: Niels Huisman, Vermont Technical College
# This is a preliminary attempt to use a raspbery pi to control a servo motor
# with the end goal of having a controlled 30 degree sweep for a sonar module
# original code was found on a youtube video by Gavin MacDonald (link below)
# and has been adapted for my purposes.
#
# original video link: https://youtu.be/ddlDgUymbc
#
# Raspberry Pi:
#   Version 3 model B
#
# Servo:
#   Generic sub-micro from Sparkfun (ROB-09065)
#       Voltage: 4.8-6.0 Volts
#       Speed: 0.15/0.10 sec/60° (4.8/6.0V)
#       Rotation: ~160°
#       3-Pole Ferrite Motor
#
# please note, I am a terrible Python programmer..

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)

p = GPIO.PWM(7,50) #Sets pin 7 to be PWM, with a freq of 50Hz
p.start(7.5)
angle = 7.5
goUp = 1
# This if/else set slows down the servo.  The servo doesn't have any speed
# control, so to create one artificially, I implemented an incremental change
# in the duty cycle with a 50ms wait in between each step
try:
    while True:
        p.ChangeDutyCycle(angle)
        time.sleep(.05)
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
    print('Stopped')
    GPIO.cleanup()

#   def moveSpeed(speed)
