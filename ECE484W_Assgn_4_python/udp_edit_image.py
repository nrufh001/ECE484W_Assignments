import cv2
import time
import os
import socket

# path to the images
original_image_path = '/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/original_image.jpg'
overlay_image_path = '/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/overlay_image.jpg'

# window name for display
window_name = 'image'

# create a socket for receiving brightness and contrast values
UDP_IP = '0.0.0.0'
UDP_PORT = 80
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

brightness = 0
contrast = 0

while True:

    # let's see if the image exists yet to display
    if os.path.exists(original_image_path):
        image = cv2.imread(original_image_path)
    else:
        image = None

    if image is not None:
        cv2.imshow(window_name, image) 

    # receive 4 byte string
    data, addr = sock.recvfrom(4)
    if data == "END":
        # Cleanup or exit as needed
        break

    # grab the current brightness and contrast values
    brightness, contrast = data[:2], data[2:]

    # display the image
    cv2.imshow(window_name, image)

    # check for user input to exit the loop
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

    # refresh the image
    time.sleep(1)

# close the window
cv2.destroyAllWindows()

