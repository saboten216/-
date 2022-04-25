import paho.mqtt.client as mqtt
import random
import json  
import datetime 
import time


# 連線設定
# 初始化地端程式
client = mqtt.Client()

# 設定連線資訊(IP, Port, 連線時間)
client.connect("127.0.0.1", 1883, 60)

while True:
    #==========傳送照片============
    f=open("6.png", "rb") #3.7kiB in same folder
    fileContent = f.read()
    byteArr = bytearray(fileContent)
    client.publish("Test1", byteArr)
    time.sleep(5)