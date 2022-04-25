import paho.mqtt.client as mqtt
import base64

# 當地端程式連線伺服器得到回應時，要做的動作
def on_connect(client, userdata, flags, rc):
    print("MQTT Broker 連線成功")
    # 將訂閱主題寫在on_connet中
    # 如果我們失去連線或重新連線時 
    # 地端程式將會重新訂閱
    client.subscribe("Test1")

# 當接收到從伺服器發送的訊息時要進行的動作
def on_message(client, userdata, msg):
    print("接收到訊息了")
    # topic = msg.topic
    # Receive_msg = msg.payload.decode('ANSI')
    # print(topic+" "+ Receive_msg)
    
    #=========接收照片並存下來===========
    photo = msg.payload
    f = open('output2.png', "wb")
    f.write(photo)
    f.close()
    
    
    
# 連線設定
# 初始化地端程式
client = mqtt.Client()
# 設定連線的動作
client.on_connect = on_connect
# 設定接收訊息的動作
client.on_message = on_message
# 設定連線資訊(IP, Port, 連線時間)
client.connect("127.0.0.1", 1883, 60)
# 開始連線，執行設定的動作和處理重新連線問題
# 也可以手動使用其他loop函式來進行連接
client.loop_forever()