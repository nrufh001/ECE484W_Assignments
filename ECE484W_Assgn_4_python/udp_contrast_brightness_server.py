import os
import socket

hostname = socket.gethostname()
IPAddr = socket.gethostbyname(hostname)

# UDP Server Info
UDP_IP = "0.0.0.0"
UDP_PORT = 80

# UDP Setup
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# UDP Server Listener
print ("READY")
while True:
    # Receive 4 byte string (4 digit number)
    data, addr = sock.recvfrom(4)
    if data == "END":
        # Cleanup
        vb.close()
        os.close(fd)
        break

    print("Received data: ", data)
