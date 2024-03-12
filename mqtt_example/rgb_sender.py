# !/usr/bin/python
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
from time import sleep
import colorsys

BROKER_ADDRESS = "127.0.0.1"
PORT = 1883
QOS = 2

def hsv2rgb(h,s,v):
    return tuple(round(i * 255) for i in colorsys.hsv_to_rgb(h,s,v))

if __name__ == "__main__":
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

    client.connect(BROKER_ADDRESS, PORT)

    print("Connected to MQTT Broker: " + BROKER_ADDRESS)
   
    for j in range(0,10000):
        for i in range(0,90):        
            hue = i / 90 
            
            for led in range(0,10):
                angle = (led*5.0) / 360.0
                hue += angle
                hue %= 1.0
                
                test_color = hsv2rgb(hue,1,1)
                DATA = "{led:" + str(led) + ",r:" + str(test_color[0]) + ",g:" + str(test_color[1]) + ",b:" + str(test_color[2]) + "}"
                client.publish("all/color", DATA, qos=QOS)
                client.loop()
            sleep(0.02)
