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


def window_capture(filename,hwnd):
  #hwnd = 0 # 窗口的编号，0号表示当前活跃窗口
  # 根据窗口句柄获取窗口的设备上下文DC（Divice Context）
  hwndDC = win32gui.GetWindowDC(hwnd)
  # 根据窗口的DC获取mfcDC
  mfcDC = win32ui.CreateDCFromHandle(hwndDC)
  # mfcDC创建可兼容的DC
  saveDC = mfcDC.CreateCompatibleDC()
  # 创建bigmap准备保存图片
  saveBitMap = win32ui.CreateBitmap()
  # 获取监控器信息
  MoniterDev = win32api.EnumDisplayMonitors(None, None)
  rect = win32gui.GetClientRect(hwnd)

  # w = MoniterDev[0][2][2]
  # h = MoniterDev[0][2][3]  
  w = rect[2]
  h = rect[3]
  # print w,h　　　#图片大小
  # 为bitmap开辟空间
  saveBitMap.CreateCompatibleBitmap(mfcDC, w, h)
  # 高度saveDC，将截图保存到saveBitmap中
  saveDC.SelectObject(saveBitMap)
  # 截取从左上角（0，0）长宽为（w，h）的图片
  saveDC.BitBlt((0, 0), (w, h), mfcDC, (0, 0), win32con.SRCCOPY)
  saveBitMap.SaveBitmapFile(saveDC, filename)

def match_pic_and_click(hwnd,img_rgb,img_gray,templateImgs,threshold = 0.8,preImg = None):
  
  # 0.程序坐标
  windowRect = win32gui.GetWindowRect(hwnd)
  windowLeft = windowRect[0]
  windowTop = windowRect[1]

  # 1.读入原图
  # img_rgb = cv.imdecode(np.fromfile(img,dtype=np.uint8),-1)
  # img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)

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
          win32gui.SetForegroundWindow(hwnd)
          

          #模拟点击
          #鼠标定位到(30,50)
          # win32api.SetCursorPos([appLeft + 50,appTop+50 ])
          # 点击中间
          win32api.SetCursorPos([windowLeft + ptLeft + w//2 ,windowTop + ptTop + h//2 ])
          #执行左单键击，若需要双击则延时几毫秒再点击一次即可
          win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP | win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0)
          # continue
          # 显示
          # cv.imshow('OpenCV/Numpy normal', img_rgb)

          # cv.waitKey()
          # cv.destroyAllWindows()
          # if cv.waitKey(25) & 0xFF == ord('q'):
          #   cv.destroyAllWindows()
          #   break

          return True
      print('未找到目标-' + templateImg)
  return False

def match_hist_pic_and_click(hwnd,name,imgHist,templateImgHist,threshold ,x,y,preImg = None):
  match = cv.compareHist(imgHist,templateImgHist,cv.HISTCMP_BHATTACHARYYA)
  if match < threshold:
    print('直方图匹配-' + name + ' - ' + str(match))
    windowRect = win32gui.GetWindowRect(hwnd)
    windowLeft = windowRect[0]
    windowTop = windowRect[1]
    win32api.SetCursorPos([windowLeft + x ,windowTop + y ])
    #执行左单键击，若需要双击则延时几毫秒再点击一次即可
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP | win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0)
    return True
  print('直方图未匹配-'+ name + ' - ' + str(match))
  return False

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

classname = "Qt5QWindowIcon"
titlename = "夜神模拟器"
#获取句柄
hwnd = win32gui.FindWindow(classname, titlename)
print(hwnd)
hwnd2 = win32gui.FindWindowEx(hwnd,None,"sub_class","sub_title")
print(hwnd2)
#获取窗口左上角和右下角坐标
rect = win32gui.GetWindowRect(hwnd)
rect2 = win32gui.GetClientRect(hwnd)
print('模拟器坐标',rect[0],rect[1])
print('模拟器窗口大小',rect2[2],rect2[3])

lastSuccess = False

configs = [
    {'imgs':["2.出击图标.jpg"] , 'threshold' : 0.9 ,'sleepTime':2 },
    {'imgs':["3.大获全胜S.jpg","3.大获全胜S2.jpg","3.战术碾压A.jpg"], 'threshold' : 0.8 ,'sleepTime':1.1 },
    {'imgs':["4.获得道具.jpg"] , 'threshold' : 0.8 ,'sleepTime':2 },
    {'imgs':["6.大获全胜2确定按钮.jpg","5.大获全胜2.jpg","5.战术碾压2.jpg"] , 'threshold' : 0.8 ,'sleepTime':2 },
    {'imgs':["8.敌人1.jpg","8.敌人2.jpg","8.敌人3.jpg","8.敌人4.jpg","8.敌人5.jpg","8.敌人6.jpg"], 'threshold' : 0.57 ,'sleepTime':2 ,'preImg' : '8.地图条件.jpg'}
  ]

img_rgb_template = cv.imdecode(np.fromfile('9.商船护卫首页.jpg',dtype=np.uint8),-1)
img_gray_template = cv.cvtColor(img_rgb_template, cv.COLOR_BGR2GRAY)
img_gray_hist_template = cv.calcHist([img_gray_template],[0],None,[256],[0,256])
cv.normalize(img_gray_hist_template, img_gray_hist_template,0,255*0.9,cv.NORM_MINMAX)
img_rgb_hist_template = cv.calcHist([img_rgb_template],[0],None,[256],[0,256])
cv.normalize(img_rgb_hist_template, img_rgb_hist_template,0,255*0.9,cv.NORM_MINMAX)

img_rgb_template2 = cv.imdecode(np.fromfile('9.海域突进首页.jpg',dtype=np.uint8),-1)
img_gray_template2 = cv.cvtColor(img_rgb_template2, cv.COLOR_BGR2GRAY)
img_gray_hist_template2 = cv.calcHist([img_gray_template2],[0],None,[256],[0,256])
cv.normalize(img_gray_hist_template2, img_gray_hist_template2,0,255*0.9,cv.NORM_MINMAX)
img_rgb_hist_template2 = cv.calcHist([img_rgb_template2],[0],None,[256],[0,256])
cv.normalize(img_rgb_hist_template2, img_rgb_hist_template2,0,255*0.9,cv.NORM_MINMAX)

img_rgb_template3 = cv.imdecode(np.fromfile('9.斩首行动首页.jpg',dtype=np.uint8),-1)
img_gray_template3 = cv.cvtColor(img_rgb_template3, cv.COLOR_BGR2GRAY)
img_gray_hist_template3 = cv.calcHist([img_gray_template3],[0],None,[256],[0,256])
cv.normalize(img_gray_hist_template3, img_gray_hist_template3,0,255*0.9,cv.NORM_MINMAX)
img_rgb_hist_template3 = cv.calcHist([img_rgb_template3],[0],None,[256],[0,256])
cv.normalize(img_rgb_hist_template3, img_rgb_hist_template3,0,255*0.9,cv.NORM_MINMAX)

img_rgb_template4 = cv.imdecode(np.fromfile('9.战术研修首页.jpg',dtype=np.uint8),-1)
img_gray_template4 = cv.cvtColor(img_rgb_template4, cv.COLOR_BGR2GRAY)
img_gray_hist_template4 = cv.calcHist([img_gray_template4],[0],None,[256],[0,256])
cv.normalize(img_gray_hist_template4, img_gray_hist_template4,0,255*0.9,cv.NORM_MINMAX)
img_rgb_hist_template4 = cv.calcHist([img_rgb_template4],[0],None,[256],[0,256])
cv.normalize(img_rgb_hist_template4, img_rgb_hist_template4,0,255*0.9,cv.NORM_MINMAX)

configHists = [{ 'name':'9.商船护卫首页.jpg','hist':img_gray_hist_template ,'rgbHist' : img_rgb_hist_template},
{ 'name':'9.海域突进首页.jpg','hist':img_gray_hist_template2 ,'rgbHist' : img_rgb_hist_template2},
{ 'name':'9.斩首行动首页.jpg','hist':img_gray_hist_template3 ,'rgbHist' : img_rgb_hist_template3},
{ 'name':'9.战术研修首页.jpg','hist':img_gray_hist_template4 ,'rgbHist' : img_rgb_hist_template4}]



def scanGame():
  sleepTime = 4
  while True:
    time.sleep(sleepTime)
    print('是否启动：【%s】' %str(checkBox1Value.get()))
    if checkBox1Value.get() == 1:
      # 1.读入截图
      window_capture("程序截图.jpg",hwnd)
      img_rgb = cv.imdecode(np.fromfile("程序截图.jpg",dtype=np.uint8),-1)
      img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)
      img_hist = cv.calcHist([img_gray],[0],None,[256],[0,256])
      cv.normalize(img_hist, img_hist,0,255*0.9,cv.NORM_MINMAX)
      img_rgb_hist = cv.calcHist([img_rgb],[0],None,[256],[0,256])
      cv.normalize(img_rgb_hist, img_rgb_hist,0,255*0.9,cv.NORM_MINMAX)

      # 2.开始扫描
      for config in configs:
        # 1.匹配图片并点击
        lastSuccess = match_pic_and_click(hwnd,img_rgb,img_gray,config['imgs'],config['threshold'],config.get('preImg'))
        if lastSuccess :
          sleepTime = config['sleepTime']
          break
      if lastSuccess:
        continue

      # 2.商船护卫
      if checkBox3Value.get() == 1:
        # match_hist_pic(img_hist,configHists[0]['hist'],'灰度匹配')
        # match_hist_pic(img_rgb_hist,configHists[0]['rgbHist'],'RGB匹配')
        lastSuccess = match_hist_pic_and_click(hwnd,configHists[0]['name'],img_hist,configHists[0]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 2.海域突进
      if checkBox4Value.get() == 1:
        # match_hist_pic(img_hist,configHists[0]['hist'],'灰度匹配')
        # match_hist_pic(img_rgb_hist,configHists[0]['rgbHist'],'RGB匹配')
        lastSuccess = match_hist_pic_and_click(hwnd,configHists[1]['name'],img_hist,configHists[1]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 2.斩首
      if checkBox5Value.get() == 1:
        # match_hist_pic(img_hist,configHists[0]['hist'],'灰度匹配')
        # match_hist_pic(img_rgb_hist,configHists[0]['rgbHist'],'RGB匹配')
        lastSuccess = match_hist_pic_and_click(hwnd,configHists[2]['name'],img_hist,configHists[2]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 2.海域突进
      if checkBox6Value.get() == 1:
        # match_hist_pic(img_hist,configHists[0]['hist'],'灰度匹配')
        # match_hist_pic(img_rgb_hist,configHists[0]['rgbHist'],'RGB匹配')
        lastSuccess = match_hist_pic_and_click(hwnd,configHists[3]['name'],img_hist,configHists[3]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 未匹配则等待5秒
      sleepTime = 4


def scanGameAdb():
  sleepTime = 4
  while True:
    time.sleep(sleepTime)
    print('是否启动：【%s】' %str(checkBox11Value.get()))
    if checkBox11Value.get() == 1:

      # 1.读入截图
      os.system("adb shell screencap -p /sdcard/screen.jpg")
      os.system("adb pull /sdcard/screen.jpg " + os.getcwd())
      img_rgb = cv.imdecode(np.fromfile("screen.jpg",dtype=np.uint8),-1)
      img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)
      img_hist = cv.calcHist([img_gray],[0],None,[256],[0,256])
      cv.normalize(img_hist, img_hist,0,255*0.9,cv.NORM_MINMAX)
      img_rgb_hist = cv.calcHist([img_rgb],[0],None,[256],[0,256])
      cv.normalize(img_rgb_hist, img_rgb_hist,0,255*0.9,cv.NORM_MINMAX)

      # 2.开始扫描
      for config in configs:
        # 1.匹配图片并点击
        lastSuccess = match_pic_and_click_adb(hwnd,img_rgb,img_gray,config['imgs'],config['threshold'],config.get('preImg'))
        if lastSuccess :
          sleepTime = config['sleepTime']
          break
      if lastSuccess:
        continue

      # # 2.商船护卫
      # if checkBox3Value.get() == 1:
      #   lastSuccess = match_hist_pic_and_click_adb(hwnd,configHists[0]['name'],img_hist,configHists[0]['hist'],0.02,500,180)
      #   if lastSuccess :
      #     sleepTime = 2
      #     continue

      # 2.海域突进
      if checkBox4Value.get() == 1:
        lastSuccess = match_hist_pic_and_click_adb(hwnd,configHists[1]['name'],img_hist,configHists[1]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 2.斩首
      if checkBox5Value.get() == 1:
        lastSuccess = match_hist_pic_and_click_adb(hwnd,configHists[2]['name'],img_hist,configHists[2]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 2.海域突进
      if checkBox6Value.get() == 1:
        lastSuccess = match_hist_pic_and_click_adb(hwnd,configHists[3]['name'],img_hist,configHists[3]['hist'],0.02,500,180)
        if lastSuccess :
          sleepTime = 2
          continue

      # 未匹配则等待5秒
      sleepTime = 4





window = tk.Tk()            #主窗口
window.title('碧蓝航线脚本(夜神模拟器)')   #窗口标题
window.geometry('350x200')  #窗口尺寸
checkBox1Value = tk.IntVar()
checkBox11Value = tk.IntVar()
checkBox2Value = tk.IntVar()
checkBox3Value = tk.IntVar()
checkBox4Value = tk.IntVar()
checkBox5Value = tk.IntVar()
checkBox6Value = tk.IntVar()
c1 = tk.Checkbutton(window, text='开始执行', variable=checkBox1Value)
c11 = tk.Checkbutton(window, text='开始执行(adb)', variable=checkBox11Value)
c2 = tk.Checkbutton(window, text='自动寻路', variable=checkBox2Value)
c3 = tk.Checkbutton(window, text='商船护卫', variable=checkBox3Value)
c4 = tk.Checkbutton(window, text='海域突进', variable=checkBox4Value)
c5 = tk.Checkbutton(window, text='斩首行动', variable=checkBox5Value)
c6 = tk.Checkbutton(window, text='战术研修', variable=checkBox6Value)
c1.pack()
c11.pack()
c2.pack()
c3.pack()
c4.pack()
c5.pack()
c6.pack()

th = threading.Thread(target=scanGame)
th.setDaemon(True)
th.start()

th2 = threading.Thread(target=scanGameAdb)
th2.setDaemon(True)
th2.start()


window.mainloop()    