# 把套件匯入
import pygsheets
import pandas as pd
import random
import time
# 載入剛剛綁定的服務帳戶金鑰並通過Oauth認證
gc = pygsheets.authorize(service_file='key.json')
# 開啟自己創建的Excel URL
sht = gc.open_by_url(
    'https://docs.google.com/spreadsheets/d/1EJIRhnuliYkFJj0pIFy_av0ko0k-cgcVWNd0kvEOrkQ/edit?usp=sharing'
    )
wks = sht[0]
print(wks)
#-------------------------------------
# 新增一筆資料(單個欄位)該如何新增
wks.update_value('A1', '***') #*** = name
# 新增多個資料(多個欄位)該如何新增
seconds = time.time()
local_time = time.ctime(seconds)
rm_te = random.randint(20, 30)

df1 = pd.DataFrame({'時間': [local_time], '溫度': [rm_te],'濕度': [70]})
# 新增進去資料表當中(copy_index代表是否要加上序號)
wks.set_dataframe(df1, 'A3', copy_index=False)
print(df1)