import serial.tools.list_ports
import time
import serial
from vpython import *
import mouse
import keyboard
import numpy as np

arduinoData = serial.Serial('com3', 9600)
while True:
    dataPacket = arduinoData.readline().decode('ascii')
    dataArray = dataPacket.split('\t')
    sensor = dataArray[0]
    if sensor == "Gyro":
        x = dataArray[1]
        y = dataArray[2]
        z = dataArray[3]
        mouse.move(-float(z), float(y), absolute=False, duration=0) 
    elif sensor == "pitchRoll":
        pitch = int(dataArray[1])
        roll = int(dataArray[2])
        if roll not in range(-9,9):
            mouse.move(-roll, 0, absolute=False, duration=0) 
        mouse.move(0, 0, absolute=False, duration=0) 
    # elif sensor == "UP":
    else:
        print(dataArray)