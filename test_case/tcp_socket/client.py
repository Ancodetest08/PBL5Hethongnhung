import socket

HOST = "172.20.10.6"
PORT = 3333

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(b"Turn on light")
print("Message sent!")
