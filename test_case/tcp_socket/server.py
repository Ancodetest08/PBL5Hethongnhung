import socket

HOST = '0.0.0.0'   # nhận từ mọi IP
PORT = 3333

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)

print(f"Server listening on {HOST}:{PORT}...")
conn, addr = s.accept()
print("Connected by", addr)

while True:
    data = conn.recv(1024)
    if not data:
        break
    print("Received:", data.decode())
    conn.sendall(b"ACK")
