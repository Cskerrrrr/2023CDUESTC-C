import struct
import numpy as np
import copy
import serial
import cv2



# ser = serial.Serial("/dev/tty5",115200)
font = cv2.FONT_HERSHEY_SIMPLEX

lower_green = np.array([35, 43, 46]) 
upper_green = np.array([77, 255, 255]) 
lower_blue = np.array([54, 54, 106])  
upper_blue = np.array([141, 255, 255]) 
lower_black = np.array([0, 0, 0]) 
upper_black = np.array([180, 255, 46]) 
lower_gray = np.array([0, 0, 46]) 
upper_gray = np.array([180, 43, 220]) 
lower_white = np.array([106, 0, 226]) 
upper_white = np.array([177, 76, 255])
lower_red_1 = np.array([0, 80, 128]) 
upper_red_1 = np.array([6, 255, 255])
lower_red = np.array([156, 43, 46]) 
upper_red = np.array([180, 255, 255])
lower_orange = np.array([11, 43, 46]) 
upper_orange = np.array([25, 255, 255])
lower_yellow = np.array([26, 43, 46])
upper_yellow = np.array([34, 255, 255])
lower_cyan = np.array([78, 43, 46])
upper_cyan = np.array([99, 255, 255])
lower_purple = np.array([125, 43, 46])
upper_purple = np.array([155, 255, 255])


graph = {'A': ['B', 'G', 'F'],
         'B': ['A', 'F', 'C'],
         'C': ['B', 'F', 'D'],
         'D': ['C','E'],
         'E': ['G', 'F', 'C', 'D'],
         'F': ['A', 'B', 'C', 'E', 'G'],
         'G': ['A', 'F', 'E']}



def send_message(message):
    data='{}'.format(message)
    ser.write(data.encode('gbk'))


def line_equation(x1, y1, x2, y2):
    A = y2 - y1
    B = x1 - x2
    C = x2*y1 - x1*y2
    
    return A, B, C

def control_car(color_x, color_y, datax, datay):
    A, B, C = line_equation(datax[0], datay[0], datax[1], datay[1])
    data = A*int(color_x[0])+B*int(color_y[0])+C   
    print(data)


def process(image):

    min_area_white=500 #280
    max_area_white=3000 #10000

    min_area_blue=2000 #280
    max_area_blue=3000 #10000

    min_area_red=280 #280
    max_area_red=10000 #10000

    outputsx_white = list()
    outputsy_white = list()
    outputsx_red = list()
    outputsy_red = list()
    outputsx_blue = list()
    outputsy_blue = list()

    outputsx = list()
    outputsy = list()
    
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    hsv = cv2.medianBlur(hsv,5)

    white = cv2.inRange(hsv, lower_white, upper_white)
    white = cv2.medianBlur(white,5)
    line = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5), (-1, -1))
    white = cv2.morphologyEx(white, cv2.MORPH_OPEN, line)
    


    contours, hierarchy = cv2.findContours(white, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if(len(contours)):
        for c in range(len(contours)):
            area = cv2.contourArea(contours[c])
            if max_area_white > area > min_area_white:       
     
                x, y, w, h = cv2.boundingRect(contours[c])
                outputsx_white.append(x + w/2)
                outputsy_white.append(y + h/2)
                outputsx.append(x + w/2)
                outputsy.append(y + h/2)
                cv2.drawContours(image, contours, c, (0, 255, 0), 2)
                cv2.putText(image,"white",(x, y), font, 1.2, (0, 0, 255), 2)
                str1 = '(' + str(int(x + w/2))+ ',' +str(int(y + h/2)) +')'
                cv2.putText(image,str1,(int(x + w/2)-50,int(y + h/2)+40),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0),2,cv2.LINE_AA)


    red_mask_1 = cv2.inRange(hsv,lower_red,upper_red)
    red_mask_2 = cv2.inRange(hsv,lower_red_1,upper_red_1)
    red_mask  = cv2.bitwise_or(red_mask_1, red_mask_2)
    red = cv2.bitwise_or(image, image, mask = red_mask)
    red = cv2.medianBlur(red,5)
    line = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5), (-1, -1))
    red = cv2.morphologyEx(red_mask, cv2.MORPH_OPEN, line)
      
    contours, hierarchy = cv2.findContours(red, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if(len(contours)):
        for c in range(len(contours)):
            area = cv2.contourArea(contours[c])
            if max_area_red > area > min_area_red:       
           
                x, y, w, h = cv2.boundingRect(contours[c])
                outputsx_red.append(x + w/2)
                outputsy_red.append(y + h/2)
                outputsx.append(x + w/2)
                outputsy.append(y + h/2)
                cv2.drawContours(image, contours, c, (0, 255, 0), 2)
                cv2.putText(image,"red",(x, y), font, 1.2, (0, 0, 255), 2)
                str1 = '(' + str(int(x + w/2))+ ',' +str(int(y + h/2)) +')' 
                cv2.putText(image,str1,(int(x + w/2)-50,int(y + h/2)+40),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0),2,cv2.LINE_AA)

    blue = cv2.inRange(hsv, lower_blue, upper_blue)
    blue = cv2.medianBlur(blue,5)
    line = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5), (-1, -1))
    blue = cv2.morphologyEx(blue, cv2.MORPH_OPEN, line)

    contours, hierarchy = cv2.findContours(blue, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if(len(contours)):
        for c in range(len(contours)):
            area = cv2.contourArea(contours[c])  
            if max_area_blue > area > min_area_blue:       
           
                x, y, w, h = cv2.boundingRect(contours[c])
                outputsx_blue.append(x + w/2)
                outputsy_blue.append(y + h/2)
                outputsx.append(x + w/2)
                outputsy.append(y + h/2)
                cv2.drawContours(image, contours, c, (0, 255, 0), 2)
                cv2.putText(image,"blue",(x, y), font, 1.2, (0, 0, 255), 2)
                str1 = '(' + str(int(x + w/2))+ ',' +str(int(y + h/2)) +')' 
                cv2.putText(image,str1,(int(x + w/2)-50,int(y + h/2)+40),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0),2,cv2.LINE_AA)        


    return image , outputsx_white, outputsy_white, outputsx_red, outputsy_red, outputsx_blue, outputsy_blue , outputsx, outputsy

def findShortestPath(graph,start,end,path=[]):
    path = path +[start]
    if start == end:
        return path
    
    shortestPath = []
    for node in graph[start]:
        if node not in path:
            newpath = findShortestPath(graph,node,end,path)
            if newpath:
                if not shortestPath or len(newpath)<len(shortestPath):
                    shortestPath = newpath
    return shortestPath


def location(outputsx_color, outputsy_color,x,y):
    x.sort()
    y.sort()  
    for i in range(len(outputsx_color)):        
        if(x[0] < outputsx_color[i] < x[3]):
            if(outputsy_color[i]> y[2]):
                cv2.putText(image,"G",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
                return 'G'
            
            else:
                cv2.putText(image,"B",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
                return 'B'
            
        elif(outputsx_color[i] <= x[0]):
            cv2.putText(image,"A",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
            return 'A'
        
        elif(outputsx_color[i] == x[3]):
            cv2.putText(image,"F",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
            return 'F'  
                
        elif(x[3] <outputsx_color[i]< x[6]):
            if(outputsy_color[i]> y[2]):
                cv2.putText(image,"E",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
                return 'E'
            
            else:
                cv2.putText(image,"C",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
                return 'C'
            
        elif(outputsx_color[i] >= x[6]):
            cv2.putText(image,"D",(int(outputsx_color[i]-10),int(outputsy_color[i]+10)), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 0), 2)
            return 'D' 
      
def locations(outputsx, outputsy):
    x = copy.deepcopy(outputsx)
    x.sort()
    y = copy.deepcopy(outputsy)
    y.sort()  
    for i in range(len(outputsx)): 
        if(x[0] < outputsx[i] < x[3]):
            if(outputsy[i]> y[2]):
                G_location = int(outputsx[i]),int(outputsy[i])

            else:
                B_location = int(outputsx[i]),int(outputsy[i])

        elif(outputsx[i] <= x[0]):
            A_location = int(outputsx[i]),int(outputsy[i])
            

        elif(outputsx[i] == x[3]):
            F_location = int(outputsx[i]),int(outputsy[i])
            

        elif(x[3] <outputsx[i]< x[6]):
            if(outputsy[i]> y[2]):
                E_location = int(outputsx[i]),int(outputsy[i])
                

            else:
                C_location = int(outputsx[i]),int(outputsy[i])
                

        elif(outputsx[i] >= x[6]):
            D_location = int(outputsx[i]),int(outputsy[i])
            


    return A_location, B_location, C_location, D_location, E_location, F_location, G_location

def draw_location(image, shortpath, A_location, B_location, C_location, D_location, E_location, F_location, G_location):
    datax = list()
    datay = list()
    for m in range(len(shortpath)):
        if(shortpath[m]=='A'):
            datax.append(int(A_location[0]))
            datay.append(int(A_location[1]))

        elif(shortpath[m]=='B'):
            datax.append(int(B_location[0]))
            datay.append(int(B_location[1]))

        elif(shortpath[m]=='C'):
            datax.append(int(C_location[0]))
            datay.append(int(C_location[1]))

        elif(shortpath[m]=='D'):
            datax.append(int(D_location[0]))
            datay.append(int(D_location[1]))

        elif(shortpath[m]=='E'):
            datax.append(int(E_location[0]))
            datay.append(int(E_location[1]))

        elif(shortpath[m]=='F'):
            datax.append(int(F_location[0]))
            datay.append(int(F_location[1]))
            
        elif(shortpath[m]=='G'):
            datax.append(int(G_location[0]))
            datay.append(int(G_location[1]))

    for k in range(len(shortpath)-1):
        cv2.line(image,(datax[k],datay[k]), (datax[k+1],datay[k+1]), (0, 255, 0), 3, 4)

    return datax, datay

def bianhuan(A_location, B_location, C_location, D_location, E_location, F_location, G_location, image):
    width,height=1280,720
    h = 40
    pts1 = np.float32([[A_location[0]-h, B_location[1]-h], [D_location[0]+h, C_location[1]-h], [A_location[0]-h, G_location[1]+h], [D_location[0]+h, E_location[1]+h]])
    pts2 = np.float32([[0,0],[width,0],[0,height],[width,height]])
    M = cv2.getPerspectiveTransform(pts1,pts2)
    image = cv2.warpPerspective(image, M, (width,height))
    return image
    
def get_location(shortpath):
    if(len(shortpath)==2):
        luxian = 5

    elif(len(shortpath)==3):
        if(shortpath[0] == 'A'):

            if(shortpath[1] == 'B'):
                    if(shortpath[2] == 'C'):#ABC
                        luxian = 2

            elif(shortpath[1] == 'G'):
                    if(shortpath[2] == 'E'):#AGE
                        luxian = 1

        elif(shortpath[0] == 'B'):

            if(shortpath[1] == 'C'):
                    if(shortpath[2] == 'D'):#BCD
                        luxian = 2

            elif(shortpath[1] == 'F'):
                    if(shortpath[2] == 'E'):#BFE
                        luxian = 5

            elif(shortpath[1] == 'A'):
                    if(shortpath[2] == 'G'):#BAG
                        luxian = 3

        elif(shortpath[0] == 'C'):

            if(shortpath[1] == 'B'):
                    if(shortpath[2] == 'A'):#CBA
                        luxian = 1

            elif(shortpath[1] == 'F'):
                    if(shortpath[2] == 'G'):#CFG
                        luxian = 5

        elif(shortpath[0] == 'D'):

            if(shortpath[1] == 'C'):
                if(shortpath[2] == 'B'):#DCB
                    luxian = 1
                elif(shortpath[2] == 'F'):#DCF
                    luxian = 3

            elif(shortpath[1] == 'E'):
                if(shortpath[2] == 'G'):#DEG
                    luxian = 2

        elif(shortpath[0] == 'E'):

            if(shortpath[1] == 'G'):
                if(shortpath[2] == 'A'):#EGA
                    luxian = 2

            elif(shortpath[1] == 'F'):
                if(shortpath[2] == 'B'):#EFB
                    luxian = 5
        elif(shortpath[0] == 'F'):

            if(shortpath[1] == 'C'):
                if(shortpath[2] == 'D'):#FCD
                    luxian = 4

        elif(shortpath[0] == 'G'):

            if(shortpath[1] == 'A'):
                if(shortpath[2] == 'B'):#GAB
                    luxian = 4

            elif(shortpath[1] == 'F'):
                if(shortpath[2] == 'C'):#GFC
                    luxian = 5

            elif(shortpath[1] == 'E'):
                if(shortpath[2] == 'D'):#GED
                    luxian = 1 
    elif(len(shortpath)==4):
        if(shortpath[0] == 'A'):
            if(shortpath[1] == 'B'):
                if(shortpath[2] == 'C'):
                    if(shortpath[3] == 'D'):#ABCD
                        luxian = 6
        elif(shortpath[0] == 'D'):
            if(shortpath[1] == 'C'):
                if(shortpath[2] == 'B'):
                    if(shortpath[3] == 'A'):#ABCD
                        luxian = 7


    return luxian       
        
    


if __name__ == '__main__':    

    image = cv2.imread('C:/Users/KK/Desktop/001.jpg')
    image = cv2.resize(image,(1280,720))    
    frame, outputsx_white, outputsy_white, outputsx_red, outputsy_red, outputsx_blue, outputsy_blue , outputsx, outputsy = process(image)
    A_location, B_location, C_location, D_location, E_location, F_location, G_location = locations(outputsx, outputsy)
    #shortpath = findShortestPath(graph,location(outputsx_red, outputsy_red, outputsx, outputsy),location(outputsx_blue, outputsy_blue, outputsx, outputsy))
    shortpath = findShortestPath(graph,'A','D')
    print('\n最短路径：',get_location(shortpath))
    datax, datay = draw_location(frame, shortpath, A_location, B_location, C_location, D_location, E_location, F_location, G_location)        
    frame = bianhuan(A_location, B_location, C_location, D_location, E_location, F_location, G_location, frame)
    while True:
        cv2.imshow("result", frame)     
        if cv2.waitKey(0) & 0xFF == ord('q'):
           break
    cv2.destroyAllWindows()
