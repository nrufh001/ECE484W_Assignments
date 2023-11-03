import mmap
import os
import time
import socket

# HPS/FPGA Memory Addresses
HW_REGS_BASE = 0xFC000000
HW_REGS_SPAN = 0x04000000
HW_REGS_MASK = HW_REGS_SPAN - 1
ALT_LWFPGASLVS_OFST = 0xFF200000
LEDS = 0x00000100
hostname = socket.gethostname()
IPAddr = socket.gethostbyname(hostname)
# UDP Server Info
UDP_IP = "192.168.1.230"
UDP_PORT = 80

# Convert 1 digit into 7-seg bit pattern
numtable = {
    "0": 0x40,
    "1": 0x79,
    "2": 0x24,
    "3": 0x30,
    "4": 0x19,
    "5": 0x12,
    "6": 0x02,
    "7": 0x78,
    "8": 0x00,
    "9": 0x10
}

# Convert list of 4 numtable into mmap writable string
def cons(bins):
    s = ""
    f = 0
    for i in range(0, 4):
        f = f | bins[i] << (i * 7)

    for i in range(0, 4):
        t = f >> (i * 8)
        s = s + chr(t & 0xFF)

    return s

# Open memory as file descriptor
fd = os.open("/dev/mem", os.O_RDWR | os.O_SYNC)

# Check if opened successfully
if fd == -1:
    print ("error opening /dev/mem!")
    exit()

# Map /dev/mem to writable block of memory
vb = mmap.mmap(
    fd,
    HW_REGS_SPAN,
    flags=mmap.MAP_SHARED,
    offset=HW_REGS_BASE
)

# 7-seg base address
pos = (ALT_LWFPGASLVS_OFST + LEDS) & HW_REGS_MASK

# Move memory block pointer to above address
vb.seek(pos)
# Display 4 number on Hex0-Hex3
def display(a, b, c, d):
    vb.write(cons([
        numtable[str(d)],
        numtable[str(c)],
        numtable[str(b)],
        numtable[str(a)]
    ]))
    vb.seek(pos)

# Sanity check
for i in range(0, 10):
    display(i, i, i, i)
    time.sleep(0.1)

# Reset
display(1, 2, 3, 4)

# UDP Setup
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

first_brightness = 0
second_brightness = 0
first_contrast = 0
second_contrast = 0

# UDP Server Listener
print ("READY")
while True:
    # Receive 3 byte string (3 digit number)
    data, addr = sock.recvfrom(3)
    if data == "END":
        # Cleanup
        vb.close()
        os.close(fd)
        break
    elif data.startswith("B"): # look for data that starts with “B”
        brightnessData = int(data[1:]) # grab the brightness data from the bits
        first_brightness = brightnessData // 10 # grabbing the first bit
        second_brightness = brightnessData % 10 # grabbing the last bit
    elif data.startswith("C"): # look for data that starts with “C”
        contrastData = int(data[1:]) # grab the data
        first_contrast = contrastData // 10 # grabbing the first contrast bit
        second_contrast = contrastData % 10 # grabbing the second bit

    print("Received data: ", data) # printing everything out
    print("Data[0]: ", data[0])
    print("Data[1]: ", data[1])
    print("Data[2]: ", data[2])
    display(first_brightness, second_brightness, first_contrast, second_contrast) # displaying the data on the display
