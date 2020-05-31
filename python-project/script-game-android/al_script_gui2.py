import os
import sys
import json
import win32gui
import win32api
import win32con, win32ui
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
import time
import argparse
import glob
import tkinter as tk
import threading
import scriptModel
from PIL import Image, ImageFilter

def match_hist_pic(imgHist,templateImgHist,name):
  match = cv.compareHist(imgHist,templateImgHist,cv.HISTCMP_BHATTACHARYYA)
  print(name + str(match))

def match_hist_pic_and_click_adb(hwnd,name,imgHist,templateImgHist,threshold ,x,y,preImg = None):
  match = cv.compareHist(imgHist,templateImgHist,cv.HISTCMP_BHATTACHARYYA)
  if match < threshold:
    print('直方图匹配-' + name + ' - ' + str(match))
    os.system( "adb shell input tap "+ str(x) +" " + str(y))

    return True
  print('直方图未匹配-'+ name + ' - ' + str(match))
  return False

def match_pic_and_click_adb(hwnd,img_rgb,img_gray,templateImgs,threshold = 0.8,preImg = None):
  

  if preImg is not None:
    template_img1 = cv.imdecode(np.fromfile(preImg,dtype=np.uint8),0)
    h, w = template_img1.shape[:2]
    res = cv.matchTemplate(img_gray, template_img1, cv.TM_CCOEFF_NORMED)
    loc = np.where(res >= 0.8) 
    if len(loc[0]) == 0 :
      print('未找到目标-' + preImg)
      return

  for templateImg in templateImgs:
      # 2.读入模板
      template_img1 = cv.imdecode(np.fromfile(templateImg,dtype=np.uint8),0)
      # template_img1 = cv.imread('2.出击图标.jpg', 0)
      h, w = template_img1.shape[:2]

      # 3.标准相关模板匹配
      res = cv.matchTemplate(img_gray, template_img1, cv.TM_CCOEFF_NORMED)

      # 4.这边是Python/Numpy的知识，后面解释
      loc = np.where(res >= threshold)  # 匹配程度大于%80的坐标y,x
      for pt in zip(*loc[::-1]):  # *号表示可选参数
          flag = True
          
          ptLeft = pt[0]
          ptTop = pt[1]
          right_bottom = (pt[0] + w, pt[1] + h)
          # 调试查看
          # cv.rectangle(img_rgb, pt, right_bottom, (0, 0, 255), 2)

          print('找到目标-' + templateImg,pt)
          # dir = 'D:/Program Files/Nox/bin/'
          os.system( "adb shell input tap "+ str(ptLeft + w//2) +" " + str(ptTop + h//2))


          return True
      print('未找到目标-' + templateImg)
  return False

#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()
templates = []
templates.append(scriptModel.Template(sift, 1,"3.大获全胜S.jpg"))
templates.append(scriptModel.Template(sift, 2,"3.大获全胜S2.jpg"))
templates.append(scriptModel.Template(sift, 3,"3.战术碾压A.jpg"))
templates.append(scriptModel.Template(sift, 4,"4.获得道具.jpg"))
templates.append(scriptModel.Template(sift, 5,"6.大获全胜2确定按钮.jpg"))
templates.append(scriptModel.Template(sift, 6,"5.大获全胜2.jpg"))

#设置Flannde参数
FLANN_INDEX_KDTREE=0
indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
searchParams= dict(checks=50)
flann=cv.FlannBasedMatcher(indexParams,searchParams)

def scanGameAdb():
  sleepTime = 4
  while True:
    time.sleep(sleepTime)
    print('是否启动：【%s】' %str(checkBox11Value.get()))
    if checkBox11Value.get() == 1:
      # 1.读入截图
      os.system("adb -s 127.0.0.1:62001 shell screencap -p /sdcard/screen.jpg")
      os.system("adb -s 127.0.0.1:62001 pull /sdcard/screen.jpg " + os.getcwd())
      img_target_rgb = cv.imdecode(np.fromfile("screen.jpg",dtype=np.uint8),-1)
      img_target_gray = cv.cvtColor(img_target_rgb, cv.COLOR_BGR2GRAY)
      kp2, features = sift.detectAndCompute(img_target_gray ,None)


      for template in templates:
        time.sleep(0.01)
        success,good = template.matchSift(flann,img_target_gray,features)
        if success:
            # 获取关键点的坐标
            src_pts = np.float32([ template.kps[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
            dst_pts = np.float32([ kp2[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
            # 计算变换矩阵和MASK
            M, mask = cv.findHomography(src_pts, dst_pts, cv.RANSAC, 5.0)
            matchesMask = mask.ravel().tolist()
            h,w = template.garyImage.shape
            # 使用得到的变换矩阵对原图像的四个角进行变换，获得在目标图像上对应的坐标
            pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
            dst = cv.perspectiveTransform(pts,M)
            cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
            os.system( "adb -s 127.0.0.1:62001 shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
        else:
            matchesMask = None
        draw_params = dict(matchColor=(0,255,0), 
                          singlePointColor=None,
                          matchesMask=matchesMask, 
                          flags=2)
        result = cv.drawMatches(template.image,template.kps,img_target_gray,kp2,good,None,**draw_params)
        
        if success:
          cv.imwrite("result.png",result)
          # im.show()
          img_screen = tk.PhotoImage(file="result.png")
          label_img.config(image=img_screen)  
          label_img.image= img_screen
          # plt.imshow(result, 'gray')
          # plt.show()

        sleepTime = 4


window = tk.Tk()            #主窗口
window.title('碧蓝航线脚本(夜神模拟器)')   #窗口标题
window.geometry('700x400')  #窗口尺寸

frameLeft = tk.Frame(window)
frameRight = tk.Frame(window)


checkBox1Value = tk.IntVar()
checkBox11Value = tk.IntVar()
checkBox2Value = tk.IntVar()
checkBox3Value = tk.IntVar()
checkBox4Value = tk.IntVar()
checkBox5Value = tk.IntVar()
checkBox6Value = tk.IntVar()
c1 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='开始执行', variable=checkBox1Value)
c11 = tk.Checkbutton(frameLeft,justify=tk.LEFT,  text='开始执行(adb)', variable=checkBox11Value)
c2 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='自动寻路', variable=checkBox2Value)
c3 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='商船护卫', variable=checkBox3Value)
c4 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='海域突进', variable=checkBox4Value)
c5 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='斩首行动', variable=checkBox5Value)
c6 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='战术研修', variable=checkBox6Value)
c1.pack()
c11.pack()
c2.pack()
c3.pack()
c4.pack()
c5.pack()
c6.pack()

img_screen = tk.PhotoImage(file="screen.jpg")
label_img = tk.Label(frameRight, image = img_screen)
label_img.pack()


frameLeft.pack(side=tk.LEFT)
frameRight.pack(side=tk.RIGHT)

th2 = threading.Thread(target=scanGameAdb)
th2.setDaemon(True)
th2.start()


window.mainloop()    