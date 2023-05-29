# -*- coding:utf-8 -*-

import cv2
import numpy as np

camera = cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,1280)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,720)
"""
功能：读取一张图片，显示出来，转化为HSV色彩空间
     并通过滑块调节HSV阈值，实时显示
"""
lower_blue = np.array([100, 26, 46])   # 颜色范围低阈值
upper_blue = np.array([124, 255, 255])  # 颜色范围高阈值
lower_white = np.array([0, 19, 175])  # 颜色范围低阈值
upper_white = np.array([130, 36, 255])  # 颜色范围高阈值
lower_red_1 = np.array([0, 80, 128]) #先找出HSV色彩空间红绿蓝三种颜色的大致范围。红色有两个是因为hsv空间中，色相h最上面和最下面都是红色。可以看下面这张图你就懂了。
upper_red_1 = np.array([6, 255, 255])
lower_red = np.array([156, 43, 46])   # 颜色范围低阈值
upper_red = np.array([180, 255, 255])  # 颜色范围高阈值

image = cv2.imread('E:/app/collect/1/1.jpg') # 根据路径读取一张图片
#image=cv2.resize(image,(1280,720))

hsv_low = np.array([0, 0, 0])
hsv_high = np.array([0, 0, 0])
hsv_low_set = np.array([100, 26, 46])
hsv_high_set = np.array([124, 255, 255])


def h_low(value):
    hsv_low[0] = value

def h_high(value):
    hsv_high[0] = value

def s_low(value):
    hsv_low[1] = value

def s_high(value):
    hsv_high[1] = value

def v_low(value):
    hsv_low[2] = value

def v_high(value):
    hsv_high[2] = value

cv2.namedWindow('image',cv2.WINDOW_AUTOSIZE)
# 可以自己设定初始值，最大值255不需要调节
cv2.createTrackbar('H low', 'image', hsv_low_set[0], 255, h_low) 
cv2.createTrackbar('H high', 'image', hsv_high_set[0], 255, h_high)
cv2.createTrackbar('S low', 'image', hsv_low_set[1], 255, s_low)
cv2.createTrackbar('S high', 'image', hsv_high_set[1], 255, s_high)
cv2.createTrackbar('V low', 'image', hsv_low_set[2], 255, v_low)
cv2.createTrackbar('V high', 'image', hsv_high_set[2], 255, v_high)

while True:

    dst = cv2.cvtColor(image, cv2.COLOR_BGR2HSV) # BGR转HSV
    dst = cv2.inRange(dst, hsv_low, hsv_high) # 通过HSV的高低阈值，提取图像部分区域
    print(hsv_low, hsv_high)
    dst = cv2.medianBlur(dst,5)
    cv2.imshow('dst', dst)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()