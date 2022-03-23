# 把套件匯入
import pygsheets
import pandas as pd
# 載入剛剛綁定的服務帳戶金鑰並通過Oauth認證
gc = pygsheets.authorize(service_file='key.json')
# 開啟自己創建的Excel URL
sht = gc.open_by_url(
    'https://docs.google.com/spreadsheets/d/1EJIRhnuliYkFJj0pIFy_av0ko0k-cgcVWNd0kvEOrkQ/edit#gid=0'
    )
# 變數wks_list等於Google Sheet的表名
wks_list = sht.worksheets()
# 把Google Sheet表名印出
print(wks_list)