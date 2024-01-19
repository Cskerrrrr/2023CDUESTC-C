#!/usr/bin/python3

from maix import image, display, camera, gpio, event
from select import select


import serial, time ,struct
camera.config(size=(240,240))
ser = serial.Serial("/dev/ttyS1", 115200, timeout=0.2)  # 连接串口

set_LAB = [[(67, 100, -14, 127, -127, 127)],  # red
           [(3, 35, 8, 127, -128, -25)]]  # blue
# LAB阈值的初始化格式:[L_MIN,A_MIN,B_MIN,L_MAX,A_MAX,B_MAX]


def check_key():
    import os
    tmp = "/dev/input/by-path/"
    if os.path.exists(tmp):
        for i in os.listdir(tmp):
            if i.find("kbd") != -1:
                return tmp + i
    return "/dev/input/event0"



count = 0
dev = event.InputDevice(check_key())


flag = 1

while True:
    
    r, w, x = select([dev], [], [], 0) # if r == 0 or set 0 will read() raise BlockingIOError 
    if r:
        for data in dev.read():
            if data.code == 0x02:
                flag = flag + 1
            if data.code == 0x03:
                flag = 1
            if data.value == 1 and data.code != 0:
                count += 1
    img = camera.capture()
    img_yuanshi = img.copy()
    img_core = img.copy()
    images = img.copy()
    img_core = img_core.Canny(threshold1 = 100, threshold2 = 180, apertureSize = 5, L2gradient = True)
    lines = img_core.find_lines(threshold=2000, x_stride = 2, y_stride = 2, theta_margin=50, rho_margin=50)
    if(flag == 3):
        img = img_yuanshi
        
    if(flag == 5):
        img = img_yuanshi
        
    if(flag == 7):
        ma = img.find_line()
        img.draw_line(ma['rect'][0], ma['rect'][1], ma['rect'][2], ma['rect'][3], (255,255,0), 1)
        img.draw_line(ma["rect"][2], ma["rect"][3], ma["rect"][4], ma["rect"][5], (255,255,0), 1)
        img.draw_line(ma["rect"][4], ma["rect"][5], ma["rect"][6], ma["rect"][7], (255,255,0), 1)
        img.draw_line(ma["rect"][6], ma["rect"][7], ma["rect"][0], ma["rect"][1], (255,255,0), 1)
        send_data = struct.pack("<BBBBBBBBBBB", 0x1c, 0x3c,ma['rect'][0], ma['rect'][1], ma['rect'][2], ma['rect'][3],ma['rect'][4],
                                ma['rect'][5],ma['rect'][6],ma['rect'][7],0xFF)
        ser.write(send_data)
    if(flag == 9):
        img.draw_line(ma['rect'][0], ma['rect'][1], ma['rect'][2], ma['rect'][3], (255,255,0), 1)
        img.draw_line(ma["rect"][2], ma["rect"][3], ma["rect"][4], ma["rect"][5], (255,255,0), 1)
        img.draw_line(ma["rect"][4], ma["rect"][5], ma["rect"][6], ma["rect"][7], (255,255,0), 1)
        img.draw_line(ma["rect"][6], ma["rect"][7], ma["rect"][0], ma["rect"][1], (255,255,0), 1)
    if(len(lines)==4 and flag == 1):
            for line in lines:
                x1 = lines[0][0]
                y1 = lines[0][1]
                x2 = lines[0][2]
                y2 = lines[0][3]
                x3 = lines[1][0]
                y3 = lines[1][1]
                x4 = lines[1][2]
                y4 = lines[1][3]
                x5 = lines[2][0]
                y5 = lines[2][1]
                x6 = lines[2][2]
                y6 = lines[2][3]
                x7 = lines[3][0]
                y7 = lines[3][1]
                x8 = lines[3][2]
                y8 = lines[3][3]
                X_center = int((lines[1][0]+lines[3][2]+lines[1][0]+lines[3][2])/4)
                Y_center = int((lines[0][1]+lines[0][3]+lines[2][1]+lines[2][1])/4)
    img.draw_line(x3, y1, x8, y2 ,color=(255, 0, 0), thickness=1)
    img.draw_line(x3, y1, x3, y5 ,color=(255, 0, 0), thickness=1)
    img.draw_line(x3, y5, x8, y5 ,color=(255, 0, 0), thickness=1)
    img.draw_line(x8, y5, x8, y2 ,color=(255, 0, 0), thickness=1) 
    for j in range(2):
        blobs = images.find_blobs(set_LAB[j])  # 在图片中查找lab阈值内的颜色色块
        if blobs:
            for i in blobs:
                size = i["w"] * i["h"]  # 最大是240 *240也就是57600
                if  40>size:
                    print(size)
                    x_start = i["x"]
                    x_end = i["x"] + i["w"]
                    x_center = int((x_start + x_end) / 2)  # 中心坐标
                    y_start = i["y"]
                    y_end = i["y"] + i["h"]
                    y_center = int((y_start + y_end) / 2)
                    img.draw_rectangle(i["x"], i["y"], i["x"] + i["w"], i["y"] + i["h"], color=(255, 0, 0), thickness = 1)  # rect
                    send_data = struct.pack("<BBBBBBB", 0x1c, 0x2c, x_center, y_center, X_center, Y_center,0xFF)
                    ser.write(send_data)
#         else:
#             send_data = struct.pack("<BBBBBBB", 0x1c, 0x2c, 0x00, 0x00, X_center, Y_center,0xFF)
#             ser.write(send_data)




    display.show(img)
