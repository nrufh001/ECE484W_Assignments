from wand.image import Image
import socket

LOCAL_UDP_IP = '127.0.0.1'
LOCAL_UDP_PORT = 7294
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_UDP_IP, LOCAL_UDP_PORT))

input_file = 'images/original_image.jpg'
output_file = 'images/adjusted_image.jpg'

brightness = 0
contrast = 0

while True:
    data, addr = sock.recvfrom(4)
    brightness, contrast = int(data[:2]), int(data[2:4])
    print("Received brightness: %d, contrast: %d" % (brightness, contrast))

    # open image
    with Image(filename=input_file) as image:

        # adjust contrast and brightness
        image.modulate(brightness=brightness, saturation=contrast, hue=100)

        # save the image
        image.save(filename=output_file)

