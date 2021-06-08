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
import tkinter.font as tkFont
import threading
import scriptModel
import scriptConfig

import subprocess
from PIL import Image, ImageFilter

import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import Spinbox
from tkinter import messagebox as mBox

def showSiftMatchImage(label_img,template,img_target_gray,kps_target,good,matchesMask):
  draw_params = dict(matchColor=(0,255,0), 
                            singlePointColor=None,
                            matchesMask=matchesMask, 
                            flags=2)
  result = cv.drawMatches(template.image,template.kps,img_target_gray,kps_target,good,None,**draw_params)
  cv.imwrite("result.png",result)
  img_screen = tk.PhotoImage(file="result.png")
  label_img.config(image=img_screen)  
  label_img.image= img_screen

#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()

config = scriptConfig.ScriptConfig()
config.AddPicConfig('点击素材','./1.图标点击',config.TemplateType.CLICK)
config.AddPicConfig('敌人素材','./1.敌人目录',config.TemplateType.CLICK)
config.AddPicConfig('特殊敌人素材','./1.敌人目录',config.TemplateType.CLICK)



#设置Flannde参数
FLANN_INDEX_KDTREE=0
indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
searchParams= dict(checks=50)
flann=cv.FlannBasedMatcher(indexParams,searchParams)


def task():
  if checkBox11Value.get() == 1:
    subprocess.getstatusoutput([entry1Value.get(),"-s",entry4Value.get(),"shell", "screencap", "-p", "/sdcard/screen.jpg"])
    subprocess.getstatusoutput([entry1Value.get(),"-s",entry4Value.get(),"pull", "/sdcard/screen.jpg", os.getcwd()])
    # os.system(entry1Value.get() + " shell screencap -p /sdcard/screen.jpg")
    # os.system(entry1Value.get() + " pull /sdcard/screen.jpg " + os.getcwd())
    img_target_rgb = cv.imdecode(np.fromfile("screen.jpg",dtype=np.uint8),-1)
    img_target_gray = cv.cvtColor(img_target_rgb, cv.COLOR_BGR2GRAY)
    kps_target, features = sift.detectAndCompute(img_target_gray ,None)
    img_hist = cv.calcHist([img_target_gray],[0],None,[256],[0,256])
    cv.normalize(img_hist, img_hist,0,255*0.9,cv.NORM_MINMAX)
    img_rgb_hist = cv.calcHist([img_target_rgb],[0],None,[256],[0,256])
    cv.normalize(img_rgb_hist, img_rgb_hist,0,255*0.9,cv.NORM_MINMAX)

    success,good,matchesMask,dst = templateMap.matchSift(flann,img_target_gray,features,kps_target)
    if success: # 判断是否在作战地图
    # 1.在战斗界面
      if checkBox5Value.get() == 1:
      # 1.1 特殊地图
        if checkBox4Value.get() == 1: # 判断是否优先Boss
          return

    


def scanGameAdb():
  sleepTime = 4
  while True:
    time.sleep(sleepTime)
    print('是否启动：【%s】' %str(checkBox11Value.get()))
    task()
    print('是否启动DC：【%s】' %str(checkBox11Value.get()))
    taskDc()
    sleepTime = 3



window = tk.Tk()           
window.title('碧蓝航线&DC脚本(夜神模拟器)')   
window.geometry('700x400')  
menubar = tk.Menu(window)
filemenu = tk.Menu(menubar, tearoff=0)
menubar.add_cascade(label='File', menu=filemenu)
filemenu.add_command(label='Exit', command=window.quit)
window.config(menu=menubar)
frameLeft = tk.Frame(window)
frameRight = tk.Frame(window)
frameBottom = tk.Frame(window)
frameLeft.pack(side=tk.LEFT, fill='y', expand='no')
frameRight.pack(side=tk.TOP, fill='x')
frameBottom.pack(side=tk.RIGHT, expand='yes', fill='both')
boldFont = tkFont.Font(family='Fixdsys', size=10, weight=tkFont.BOLD)

checkBox1Value = tk.IntVar()
checkBox11Value = tk.IntVar()
checkBox2Value = tk.IntVar()
checkBox3Value = tk.IntVar()
checkBox4Value = tk.IntVar()
checkBox5Value = tk.IntVar()
checkBox6Value = tk.IntVar()
checkBox7Value = tk.IntVar()
checkBox8Value = tk.IntVar()
checkBox109Value = tk.IntVar()
checkBox110Value = tk.IntVar()
checkBox111Value = tk.IntVar()
checkBox201Value = tk.IntVar()
checkBox202Value = tk.IntVar()
entry1Value = tk.StringVar(value='C:/leidian/LDPlayer4/adb')
entry4Value = tk.StringVar(value='emulator-5556')
entry2Value = tk.StringVar(value='500,180')
entry3Value = tk.StringVar(value='500,180')
entry21Value = tk.StringVar(value='C:/leidian/LDPlayer64/adb')
entry22Value = tk.StringVar(value='emulator-5554')

tk.Label(frameLeft,text="碧蓝航线",font=boldFont).pack()
# tk.Checkbutton(frameLeft, justify=tk.LEFT, text='开始执行', variable=checkBox1Value).pack()
tk.Checkbutton(frameLeft,justify=tk.LEFT,  text='开始执行(adb)', variable=checkBox11Value).pack()
# tk.Checkbutton(frameLeft, justify=tk.LEFT, text='自动寻路', variable=checkBox2Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='任务界面', variable=checkBox3Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='优先BOSS', variable=checkBox4Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='3-4循环', variable=checkBox109Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特殊敌人', variable=checkBox110Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='是否连续点击', variable=checkBox111Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特殊地图', variable=checkBox5Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯', variable=checkBox6Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯(普通)', variable=checkBox7Value).pack()
tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯(简单)', variable=checkBox8Value).pack()
tk.Label(frameLeft,text="DC",font=boldFont).pack()
tk.Checkbutton(frameLeft,justify=tk.LEFT,  text='开始执行(adb)', variable=checkBox201Value).pack()
tk.Checkbutton(frameLeft,justify=tk.LEFT,  text='自动raid', variable=checkBox202Value).pack()

tk.Label(frameBottom,text="命令地址:").grid(row=0,column=0)
tk.Entry(frameBottom,textvariable = entry1Value).grid(row=0,column=1,columnspan=2,sticky=tk.EW)
tk.Entry(frameBottom,textvariable = entry4Value).grid(row=0,column=3)
tk.Label(frameBottom,text="DC命令地址:").grid(row=2,column=0)
tk.Entry(frameBottom,textvariable = entry1Value).grid(row=2,column=1,columnspan=2,sticky=tk.EW)
tk.Entry(frameBottom,textvariable = entry22Value).grid(row=2,column=3)
tk.Label(frameBottom,text="普通任务坐标:").grid(row=1,column=0)
tk.Label(frameBottom,text="战术研修坐标:").grid(row=1,column=2)
tk.Entry(frameBottom,textvariable = entry2Value).grid(row=1,column=1)
tk.Entry(frameBottom,textvariable = entry3Value).grid(row=1,column=3)


tree = ttk.Treeview(frameBottom, columns=('name','path'))
tree.grid(row=3,column=0,columnspan=4)
tree.column("name", width=100)        
tree.column("path", width=100)
tree.heading("name", text="名称")        # #设置显示的表头名
tree.heading("path", text="路径")
# tree.insert("", 0, text="line1", values=("卡恩", "18", "180", "65"))  
# tree.insert("", 0, text="line1", values=("卡恩", "18", "180", "65"))  

img_screen = tk.PhotoImage(file="screen.jpg")
label_img = tk.Label(frameRight, image = img_screen)
label_img.pack()
th2 = threading.Thread(target=scanGameAdb)
th2.setDaemon(True)
th2.start()
window.mainloop()    