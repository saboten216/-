import paho.mqtt.client as mqtt
import base64
import pygsheets
import pandas as pd
import datetime 
import time
# 當地端程式連線伺服器得到回應時，要做的動作



def on_connect(client, userdata, flags, rc):
    print("MQTT Broker 連線成功")
    # 將訂閱主題寫在on_connet中
    # 如果我們失去連線或重新連線時 
    # 地端程式將會重新訂閱
    client.subscribe("xxxxxxxxxxx/xxxxxxxxxxx")

# 當接收到從伺服器發送的訊息時要進行的動作
i = 1
def on_message(client, userdata, msg):
    global i
    print("接收到訊息了")
    topic = msg.topic
    Receive_msg = msg.payload.decode('ANSI')
    print(topic+" "+ Receive_msg)
    updata(Receive_msg , i)
    i = i + 1
    

def updata(data , i):
    ISOTIMEFORMAT = '%m/%d %H:%M:%S'
    t = datetime.datetime.now().strftime(ISOTIMEFORMAT)
    # 載入剛剛綁定的服務帳戶金鑰並通過Oauth認證
    gc = pygsheets.authorize(service_file='key.json')
    # 開啟自己創建的Excel URL
    sht = gc.open_by_url(
        'https://docs.google.com/spreadsheets/d/12nh3EaRz9wa4FMR0Uzv3iqxo3zb-Y4DewbsGLv4hUG0/edit#gid=0'
        )
    wks = sht[0]
    wks.update_value(f'A{i}', data)
    wks.update_value(f'B{i}', t)

    
    
# 連線設定
# 初始化地端程式
client = mqtt.Client()
# 設定連線的動作
client.on_connect = on_connect
# 設定接收訊息的動作
client.on_message = on_message
# 設定連線資訊(IP, Port, 連線時間)
client.username_pw_set(username="xxxxx", password="xxxxx")
client.connect("xxx.xx.xx.xxx", 1883, 60)
# 開始連線，執行設定的動作和處理重新連線問題
# 也可以手動使用其他loop函式來進行連接
client.loop_forever()
