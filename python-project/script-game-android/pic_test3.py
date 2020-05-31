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
  

template  = cv.imdecode(np.fromfile('test.jpg',dtype=np.uint8),0)
target  = cv.imdecode(np.fromfile('9.战术研修首页.jpg',dtype=np.uint8),0)

#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()
kp1, des1 = sift.detectAndCompute(template,None)
kp2, des2 = sift.detectAndCompute(target ,None)


#设置Flannde参数
FLANN_INDEX_KDTREE=0
indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
searchParams= dict(checks=50)
flann=cv.FlannBasedMatcher(indexParams,searchParams)

matches=flann.knnMatch(des1,des2,k=2)

MIN_MATCH_COUNT = 10 #设置最低特征点匹配数量为10
good = []
#舍弃大于0.7的匹配
for m,n in matches:
    if m.distance < 0.66 *n.distance:
        good.append(m)
if len(good) > MIN_MATCH_COUNT:
    # 获取关键点的坐标
    src_pts = np.float32([ kp1[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
    dst_pts = np.float32([ kp2[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
    # 计算变换矩阵和MASK
    M, mask = cv.findHomography(src_pts, dst_pts, cv.RANSAC, 5.0)
    matchesMask = mask.ravel().tolist()
    h,w = template.shape
    # 使用得到的变换矩阵对原图像的四个角进行变换，获得在目标图像上对应的坐标
    pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
    dst = cv.perspectiveTransform(pts,M)
    cv.polylines(target,[np.int32(dst)],True,0,2, cv.LINE_AA)
else:
    print( "Not enough matches are found - %d/%d" % (len(good),MIN_MATCH_COUNT))
    matchesMask = None
draw_params = dict(matchColor=(0,255,0), 
                   singlePointColor=None,
                   matchesMask=matchesMask, 
                   flags=2)
result = cv.drawMatches(template,kp1,target,kp2,good,None,**draw_params)
plt.imshow(result, 'gray')
plt.show()