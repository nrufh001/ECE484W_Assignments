import cv2
import time
import os
import socket
import threading

LOCAL_UDP_IP = '127.0.0.1'
LOCAL_UDP_PORT = 7294

overlay = 0

# flag to control the threads
exit_flag = False

window_name = "image"
original_image_path = r'/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/images/original_image.jpg'
edited_image_path = r'/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/images/edited_image.jpg'
result_image_path = r'/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/images/result_image.jpg'

# updates and displays the image
def update_image():
    global exit_flag
    global overlay
    while not exit_flag:
#        print("overlay: ", overlay)
        if os.path.exists(result_image_path) and overlay == 1:
            image = cv2.imread(result_image_path)
        else:
            image = cv2.imread(original_image_path)
        cv2.imshow(window_name, image)
        cv2.waitKey(1)
        time.sleep(1)  # Update the image every second

# brightness values    --> contrast values      --> overlay
# [0, 0] to [100, 100] --> [0, 0] to [100, 100] --> [0] or [1]
# receives brightness and contrast values
def receive_data():
    global exit_flag
    global overlay
    UDP_IP = '0.0.0.0'
    UDP_PORT = 80
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    while not exit_flag:
        data, addr = sock.recvfrom(5)
        if data == "END":
            exit_flag = True
        else:
            brightness, contrast, overlay = int(data[:2]), int(data[2:4]), int(data[4:])
            overlay_image(overlay)
	    edit_image(brightness, contrast)
            print("brightness: ", brightness)
            print("contrast: ", contrast)
            print("overlay: ", overlay)

def edit_image(brightness, contrast):
    print("b and c to send: ", brightness, contrast)
    data_to_send = '{:02d}{:02d}'.format(int(brightness), int(contrast))
    print("data to send: ", data_to_send) 
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(data_to_send.encode(), (LOCAL_UDP_IP, LOCAL_UDP_PORT))

def overlay_image(overlay_state):
    # load images
    original_image = cv2.imread('images/original_image.jpg')
    overlay_image = cv2.imread('images/overlay_image.jpg')

    if(overlay_state == 1): # if overlay is off then
	# size of images
	width, height = 200, 200

	# this is super weird, but just resizing the overlay image
	# so that everything fits nicely together
	ar = float(width) / overlay_image.shape[1] # get the aspect ratio
	adjusted_height = int(overlay_image.shape[0] * ar) # get the new height
	overlay_image = cv2.resize(overlay_image, (width, adjusted_height)) # and finally resize it

	# just grabbing the new overlay demensions
	overlay_height, overlay_width, _ = overlay_image.shape

	# coordinates to place in top left corner of the image
	# can always change to move the image around
	x, y = 50, 50

	# grabbing pts on original image for the overlay image to be placed
	pts = original_image[y:y+overlay_height, x:x+overlay_width]

	# placing overlay image onto the original
	result = cv2.addWeighted(pts, 0, overlay_image, 1, 0)

	# replacing the original pts with the result
	original_image[y:y+overlay_height, x:x+overlay_width] = result

	# and FINALLY saving the image
	cv2.imwrite('images/result_image.jpg', original_image)

# gonna have to use threads
# starting image thread
image_thread = threading.Thread(target=update_image)
image_thread.start()

# starting data thread
data_thread = threading.Thread(target=receive_data)
data_thread.start()

# wait for the threads to finish
image_thread.join()
data_thread.join()

# and then close the window
cv2.destroyAllWindows()

