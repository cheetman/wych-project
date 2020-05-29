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
  

# 用于显示图片的灰度直方图
def hist(img):
	hist2 = cv.calcHist([img],[0],None,[256],[0,256])
	plt.subplot(121)
	plt.imshow(img,'gray')
	plt.xticks([])
	plt.yticks([])
	plt.title("Original")
	plt.subplot(122)
	plt.hist(img.ravel(),256,[0,256])
	plt.show()

# 1.读取图片
img_rgb = cv.imdecode(np.fromfile('测试图片.jpg',dtype=np.uint8),-1)
# 2.转为灰度图
img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)
# 3.保存灰度图
cv.imencode('.jpg', img_gray)[1].tofile('灰度图.jpg')
# 4.二值化
ret,thresh = cv.threshold(img_gray,127,255,cv.THRESH_BINARY)
# 5.保存二值化图
cv.imencode('.jpg', thresh)[1].tofile('二值化图.jpg')



img_rgb1 = cv.imdecode(np.fromfile('程序截图.jpg',dtype=np.uint8),-1)
img_rgb2 = cv.imdecode(np.fromfile('9.商船护卫首页.jpg',dtype=np.uint8),-1)
img_rgb3 = cv.imdecode(np.fromfile('直方图测试3.jpg',dtype=np.uint8),-1)
img_gray1 = cv.cvtColor(img_rgb1, cv.COLOR_BGR2GRAY)
img_gray2 = cv.cvtColor(img_rgb2, cv.COLOR_BGR2GRAY)
img_gray3 = cv.cvtColor(img_rgb3, cv.COLOR_BGR2GRAY)
hist1 = cv.calcHist([img_gray1],[0],None,[256],[0,256])
hist2 = cv.calcHist([img_gray2],[0],None,[256],[0,256])
hist3 = cv.calcHist([img_gray3],[0],None,[256],[0,256])
cv.normalize(hist1, hist1,0,255*0.9,cv.NORM_MINMAX)
cv.normalize(hist2, hist2,0,255*0.9,cv.NORM_MINMAX)
cv.normalize(hist3, hist3,0,255*0.9,cv.NORM_MINMAX)




match1 = cv.compareHist(hist1,hist2,cv.HISTCMP_BHATTACHARYYA)
match2 = cv.compareHist(hist1,hist3,cv.HISTCMP_BHATTACHARYYA)
match1 = cv.compareHist(hist1,hist2,cv.HISTCMP_CORREL)
match2 = cv.compareHist(hist1,hist3,cv.HISTCMP_CORREL)
match1 = cv.compareHist(hist1,hist2,cv.HISTCMP_CHISQR)
match2 = cv.compareHist(hist1,hist3,cv.HISTCMP_CHISQR)
match1 = cv.compareHist(hist1,hist2,cv.HISTCMP_INTERSECT)
match2 = cv.compareHist(hist1,hist3,cv.HISTCMP_INTERSECT)


# plt.subplot(2, 1, 1)
# plt.plot(hist1)
# plt.subplot(2, 1, 2)
# plt.plot(hist2)
# # plt.subplot(2, 1, 3)
# # plt.plot(hist3)
# plt.show()




# match3 = cv.compareHist(hist1,hist2,cv.HISTCMP_BHATTACHARYYA)
# #获取灰度图矩阵的行数和列数
# r,c = img_gray.shape[:2]
# dark_sum=0	#偏暗的像素 初始化为0个
# dark_prop=0	#偏暗像素所占比例初始化为0
# piexs_sum=r*c	#整个弧度图的像素个数为r*c

# #遍历灰度图的所有像素
# for row in img_gray:
#     for colum in row:
#         if colum<40:	#人为设置的超参数,表示0~39的灰度值为暗
#             dark_sum+=1
# dark_prop=dark_sum/(piexs_sum)
# print("偏暗的像素:"+str(dark_sum))
# print("所有像素:"+str(piexs_sum))
# print("偏暗像素所占比例=偏暗的像素/所有像素:"+str(dark_prop))
# if dark_prop >=0.75:	#人为设置的超参数:表示若偏暗像素所占比例超过0.78,则这张图被认为整体环境黑暗的图片
#     print("图片 is dark!")
# else:
#     print("图片 is bright!")
# hist(img_gray);  #若要查看图片的灰度值分布情况,可以这个注释解除
