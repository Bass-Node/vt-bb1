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

try:
    while True:
# without the assignment of p above, and setting it to start at 7.5,
# these four lines would send a quick, 1.5ms pulse to the servo, encouraging
# it to return to the neutral position.  This was step one to getting the
# pi to control the servo.      
#        GPIO.output(7,1)
#        time.sleep(0.0015)
#        GPIO.output(7,0)
#        time.sleep(2)

        p.ChangeDutyCycle(7.5)
        time.sleep(1)
        p.ChangeDutyCycle(9)
        time.sleep(1)
        p.ChangeDutyCycle(6.5)
        time.sleep(1)

except KeyboardInterrupt:
    print('Stopped')
    GPIO.cleanup()
