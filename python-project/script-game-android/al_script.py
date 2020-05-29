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

def match_pic_and_click(hwnd,img,templateImgs,threshold = 0.8,preImg = None):
  
  # 0.程序坐标
  windowRect = win32gui.GetWindowRect(hwnd)
  windowLeft = windowRect[0]
  windowTop = windowRect[1]

  # 1.读入原图
  img_rgb = cv.imdecode(np.fromfile(img,dtype=np.uint8),-1)
  img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)

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
          cv.rectangle(img_rgb, pt, right_bottom, (0, 0, 255), 2)

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
          cv.imshow('OpenCV/Numpy normal', img_rgb)

          # cv.waitKey()
          # cv.destroyAllWindows()
          if cv.waitKey(25) & 0xFF == ord('q'):
            cv.destroyAllWindows()
            break

          return True
      print('未找到目标-' + templateImg)
  return False






# Initialize the parameters
# confThreshold = 0.5  #Confidence threshold
# nmsThreshold = 0.4   #Non-maximum suppression threshold置信度阈值
# inpWidth = 320       #Width of network's input image，改为320*320更快
# inpHeight = 320      #Height of network's input image，改为608*608更准

# parser = argparse.ArgumentParser(description='Object Detection using YOLO in OPENCV')
# parser.add_argument('--image', help='Path to image file.')
# parser.add_argument('--video', help='Path to video file.')
# args = parser.parse_args()


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
sleepTime = 4

#开始循环
while True:

  time.sleep(sleepTime)

  print('开始循环')
  #截取全屏
  window_capture("程序截图.jpg",hwnd)


  configs = [
      {'imgs':["2.出击图标.jpg"] , 'threshold' : 0.9 ,'sleepTime':2 },
      {'imgs':["3.大获全胜S.jpg","3.大获全胜S2.jpg","3.战术碾压A.jpg"], 'threshold' : 0.8 ,'sleepTime':1.1 },
      {'imgs':["4.获得道具.jpg"] , 'threshold' : 0.8 ,'sleepTime':2 },
      {'imgs':["6.大获全胜2确定按钮.jpg","5.大获全胜2.jpg","5.战术碾压2.jpg"] , 'threshold' : 0.8 ,'sleepTime':2 },
      {'imgs':["8.敌人1.jpg","8.敌人2.jpg","8.敌人3.jpg"], 'threshold' : 0.57 ,'sleepTime':2 ,'preImg' : '8.地图条件.jpg'}
    ]

  for config in configs:

      # 1.匹配图片并点击
    lastSuccess = match_pic_and_click(hwnd,"程序截图.jpg",config['imgs'],config['threshold'],config.get('preImg'))
    if lastSuccess :
      sleepTime = config['sleepTime']
      break

    # 未匹配则等待5秒
    sleepTime = 4

  # template_img2 = cv.imread('sucai2.jpg', 0)
  # h, w = template_img2.shape[:2]
  # res = cv.matchTemplate(img_gray, template_img2, cv.TM_CCOEFF_NORMED)
  # threshold = 0.8 
  # loc = np.where(res >= threshold)  
  # flag = False;
  # for pt in zip(*loc[::-1]): 
  #     flag = True;
  #     print('找到目标-获得道具 坐标',pt)
  #     win32gui.SetForegroundWindow(hwnd)
  #     win32api.SetCursorPos([appLeft + 50,appTop+50 ])
  #     win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP | win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0)
  #     break
  # if flag == False :
  #   print('未找到目标-获得道具')

  # template_img3 = cv.imread('sucai3_1.jpg', 0)
  # h, w = template_img3.shape[:2]
  # res = cv.matchTemplate(img_gray, template_img3, cv.TM_CCOEFF_NORMED)
  # threshold = 0.8 
  # loc = np.where(res >= threshold)  
  # flag = False;
  # for pt in zip(*loc[::-1]): 
  #     flag = True;
  #     print('找到目标-大获全胜2 确认按钮 坐标',pt)
  #     win32gui.SetForegroundWindow(hwnd)
  #     win32api.SetCursorPos([pt[0] + 10,pt[1] + 10 ])
  #     win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP | win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0)
  #     break
  # if flag == False :
  #   print('未找到目标-大获全胜2 确认按钮')







# left_top = max_loc  # 左上角
# right_bottom = (left_top[0] + w, left_top[1] + h)  # 右下角
# print(left_top)
# print(right_bottom)

# cv.rectangle(img, left_top, right_bottom, 255, 2)  # 画出矩形位置

# cv.imshow('hahajpg',img_rgb)
# cv.waitKey(0)
# cv.destroyAllWindows()






# 指定句柄设置为前台，也就是激活
#win32gui.SetForegroundWindow(hwnd)
# 设置为后台
# win32gui.SetBkMode(hwnd, win32con.TRANSPARENT)
# itchat.send('Hello, filehelper', toUserName='filehelper')


# template  = cv.imread('img1.jpg',0)          # queryImage
# img = cv.imread('imgb2.jpg',0) # trainImage
# h, w = template.shape[:2]  # rows->h, cols->w

# # 相关系数匹配方法：cv2.TM_CCOEFF
# res = cv.matchTemplate(img, template, cv.TM_CCOEFF)
# print('匹配度',res)
# threshold = 0.8
# loc = np.where(res >= threshold) 
# print('匹配度0.8',loc)
# # min_val, max_val, min_loc, max_loc = cv.minMaxLoc(res)

# for pt in zip(*loc[::-1]):  # *号表示可选参数
#     right_bottom = (pt[0] + w, pt[1] + h)
#     cv.rectangle(img, pt, right_bottom, (0, 0, 255), 2)




# Initiate ORB detector
# orb = cv.ORB_create()
# # find the keypoints and descriptors with ORB


# kp1, des1 = orb.detectAndCompute(img1,None)
# kp2, des2 = orb.detectAndCompute(img2,None)


# # create BFMatcher object
# bf = cv.BFMatcher(cv.NORM_HAMMING, crossCheck=True)
# # Match descriptors.
# matches = bf.match(des1,des2)
# # Sort them in the order of their distance.
# matches = sorted(matches, key = lambda x:x.distance)
# print(matches)

# for item in matches:        # 第二个实例
#    print(item.distance,item.trainIdx ,item.queryIdx  ,item.imgIdx  )


# # Draw first 10 matches.
# img3 = cv.drawMatches(img1,kp1,img2,kp2,matches[:10],None, flags=2)
# plt.imshow(img3),plt.show()


# window_capture("haha.jpg",hwnd)

# # Load an color image in grayscale
# img = cv.imread('haha.jpg',0)
# cv.imshow('hahajpg',img)
# cv.waitKey(0)
# cv.destroyAllWindows()



# MIN_MATCH_COUNT = 10

# # Initiate SIFT detector
# sift = cv.xfeatures2d.SIFT_create()
# # find the keypoints and descriptors with SIFT
# kp1, des1 = sift.detectAndCompute(img1,None)
# kp2, des2 = sift.detectAndCompute(img2,None)
# FLANN_INDEX_KDTREE = 1
# index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
# search_params = dict(checks = 50)
# flann = cv.FlannBasedMatcher(index_params, search_params)
# matches = flann.knnMatch(des1,des2,k=2)
# # store all the good matches as per Lowe's ratio test.
# good = []
# for m,n in matches:
#     if m.distance < 0.7*n.distance:
#         good.append(m)

# if len(good)>MIN_MATCH_COUNT:
#     src_pts = np.float32([ kp1[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
#     dst_pts = np.float32([ kp2[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
#     M, mask = cv.findHomography(src_pts, dst_pts, cv.RANSAC,5.0)
#     matchesMask = mask.ravel().tolist()
#     h,w,d = img1.shape
#     pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
#     dst = cv.perspectiveTransform(pts,M)
#     img2 = cv.polylines(img2,[np.int32(dst)],True,255,3, cv.LINE_AA)
# else:
#     print( "Not enough matches are found - {}/{}".format(len(good), MIN_MATCH_COUNT) )
#     matchesMask = None


# draw_params = dict(matchColor = (0,255,0), # draw matches in green color
#                    singlePointColor = None,
#                    matchesMask = matchesMask, # draw only inliers
#                    flags = 2)
# img3 = cv.drawMatches(img1,kp1,img2,kp2,good,None,**draw_params)
# plt.imshow(img3, 'gray'),plt.show()
