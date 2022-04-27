# 課堂記錄用
## 0302

- 點亮MCU上面內建的燈
- 閃爍5次
- 持續閃爍

## 0309

- 透過Serial，控制led亮滅
- 溫度28度以上時，LED亮；28度以下熄滅 `測試調整成26度`
- 濕度達到28度以上，測量九軸姿態 `濕度不好控制 測試使用溫度`

## 0316

- 蒐集溫溼度，繪製圖表 `google sheet`
- 加上序號
- 加上時間
- [溫溼度紀錄表](https://docs.google.com/spreadsheets/d/1Loe5P8JnAEPR9dCbh2FyJZVk8SKiVBj4O5C-Pu08fbg/edit?usp=sharing)

## 0323

- 利用api&python在Google Sheet添加一筆資料
- 在Google Sheet 新增10筆資料 `random`
- 使用ESP32新增資料 `arduino語法`

## 0330

- Websocket `python` `javascript`

## 0413

- MQTT `python`
- [課程用code參考](https://ithelp.ithome.com.tw/articles/10227131)
- 定時(3秒)透過 MQTT 發送訊息給組員,訊息格式：{‘StudentID’ : “你的學號” , ‘Time’ : 當前電腦時間}
- 改寫MQTT_Subscribe.py，組員publish photo，收到後儲存至本地


## 0420

- NB-IOT `SIM 7000e`
- [參考](https://frankchang.me/2018/12/18/sim7000c/)
- 發送AT，回傳OK
- SIM7000e 回應OK 後亮紅燈，連網則熄滅紅燈

## 0427

- Python訂閱MQTT後，取得主題下的數值，傳輸至google sheet，將Google sheet的資料做成圖表

- [MQTT溫溼度紀錄](https://docs.google.com/spreadsheets/d/12nh3EaRz9wa4FMR0Uzv3iqxo3zb-Y4DewbsGLv4hUG0/edit#gid=0)
