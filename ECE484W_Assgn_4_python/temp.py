import socket

# Define the UDP server information
UDP_IP = "0.0.0.0"
UDP_PORT = 80

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # Bind the socket to the specified IP and port
    sock.bind((UDP_IP, UDP_PORT))
    print("UDP server is ready. Listening on", UDP_IP, "port", UDP_PORT)
except socket.error as e:
    print("Error binding the UDP socket:", e)
    sock.close()
    exit(1)

# Initialize variables to store brightness and contrast values
first_brightness = 0
second_brightness = 0
first_contrast = 0
second_contrast = 0

while True:
    try:
        # Receive data from the client
        data, addr = sock.recvfrom(3)

        if data == b"END":
            print("Received END signal. Exiting.")
            break
        elif data.startswith(b"B"):
            # Parse brightness data
            brightnessData = int(data[1:])
            first_brightness = brightnessData // 10
            second_brightness = brightnessData % 10
        elif data.startswith(b"C"):
            # Parse contrast data
            contrastData = int(data[1:])
            first_contrast = contrastData // 10
            second_contrast = contrastData % 10

        print("Received data:", data.decode())
        print("Brightness: {}{}, Contrast: {}{}".format(
            first_brightness, second_brightness, first_contrast, second_contrast
        ))
    except socket.error as e:
        print("Error receiving data:", e)

# Close the socket
sock.close()

