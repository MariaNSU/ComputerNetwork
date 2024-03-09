import socket
MAX_BUF_SIZE = 1024
request = b'GET /.txt HTTP/1.1'
host = "127.0.0.1"
port = 20000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.send(request)
message = s.recv(MAX_BUF_SIZE)
print('Received from server: ')
while message:
    split_mes = message.split(b'\r\n')
    for mes in split_mes:
        print(mes.decode())
    message = s.recv(MAX_BUF_SIZE)
