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
  

img_rgb1 = cv.imdecode(np.fromfile('QQ截图20200529234557.jpg',dtype=np.uint8),0)
img_rgb2 = cv.imdecode(np.fromfile('9.商船护卫首页.jpg',dtype=np.uint8),0)

sift=cv.xfeatures2d.SIFT_create()#创建sift检测器
kp1, des1 = sift.detectAndCompute(img_rgb1,None)
kp2, des2 = sift.detectAndCompute(img_rgb2,None)

#设置Flannde参数
FLANN_INDEX_KDTREE=0
indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
searchParams= dict(checks=50)
flann=cv.FlannBasedMatcher(indexParams,searchParams)
matches=flann.knnMatch(des1,des2,k=2)

#设置好初始匹配值
matchesMask=[[0,0] for i in range (len(matches))]
for i, (m,n) in enumerate(matches):
	if m.distance< 0.5*n.distance: #舍弃小于0.5的匹配结果
		matchesMask[i]=[1,0]
drawParams=dict(matchColor=(0,0,255),singlePointColor=(255,0,0),matchesMask=matchesMask,flags=0) #给特征点和匹配的线定义颜色
resultimage=cv.drawMatchesKnn(img_rgb1,kp1,img_rgb2,kp2,matches,None,**drawParams) #画出匹配的结果
plt.imshow(resultimage,),plt.show()