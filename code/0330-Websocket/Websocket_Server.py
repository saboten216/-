from websocket_server import WebsocketServer

# Called for every client connecting (after handshake)
def new_client(client, server):
    print(client)
    server.send_message_to_all("New Client:%s" % ("id：" + str(client['id'])))


# Called for every client disconnecting
def client_left(client, server):
    print("Client(%s) disconnected" % "id：" + str(client['id']))


# Called when a client sends a message
def message_received(client, server, message):
    
    # message = int(message) #傳出去的數字變成2倍 再傳到聊天室
    # message = message * 2
    # message = str(message)
    
    if len(message) > 200:
        message = message[:200]+'..'
    # message = int(message) + 1
    server.send_message_to_all("Client(%d) said: %s" % (client['id'], str(message)))


server = WebsocketServer(host='0.0.0.0', port=8765)
server.set_fn_new_client(new_client)
server.set_fn_client_left(client_left)
server.set_fn_message_received(message_received)
server.run_forever()