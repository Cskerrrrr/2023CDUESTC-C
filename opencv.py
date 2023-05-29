import cv2
CAM_WIDTH = 1280
CAM_HEIGHT = 720


camera = cv2.VideoCapture(0)
#camera.set(cv2.CAP_PROP_FPS,30)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,CAM_WIDTH)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,CAM_HEIGHT)

while True:
    ret,frame = camera.read()
    frame = cv2.resize(frame,(1280,720)) 
    #frame = cv2.flip(frame, 1)
    cv2.imshow('video',frame)
    if cv2.waitKey(1) & 0xFF == ord('s'):
        saveFile = 'C:/Users/KK/Desktop/001.jpg'
        cv2.imwrite(saveFile, frame)  # imwrite 不支持中文路径和文件名，读取失败，但不会报错!
        break
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
camera.release()
cv2.destroyAllWindows()
