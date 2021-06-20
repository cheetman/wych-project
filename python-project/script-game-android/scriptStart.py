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
import scriptUI
import re

import subprocess
from PIL import Image, ImageFilter

import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import Spinbox
from tkinter import messagebox as mBox


ui = scriptUI.ScriptUI()


config = scriptConfig.ScriptConfig(ui.tree2)
config.AddPicConfig('点击素材','./1.图标点击',config.TemplateType.CLICK)
config.AddPicConfig('敌人素材','./8.敌人目录',config.TemplateType.CLICK)
config.AddPicConfig('特殊敌人素材','./9.特殊敌人目录',config.TemplateType.CLICK)

ui.templateConfig = config

# img_screen = tk.PhotoImage(file="screen.jpg")
# ui.img.config(image=img_screen)  
# ui.img.image= img_screen



def scan():
    
    sift=cv.xfeatures2d.SIFT_create()

    #Flannde参数
    FLANN_INDEX_KDTREE=0
    indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
    searchParams= dict(checks=50)
    flann=cv.FlannBasedMatcher(indexParams,searchParams)

    sleepTime = 10
    while True:
        time.sleep(sleepTime)
        sleepTime = int(ui.configs['entryrate'].get())
        cbstartdebugFlag = ui.configs['cbstartdebug'].get()
        if ui.configs['cbstart'].get() == 1 or cbstartdebugFlag == 1 :
            print('启动扫描......')
            # 截图
            adbpath = ui.configs["entryadb"].get()
            device = ui.getDevice()
            screenFileName = "screen.png"
            if device is not None or  cbstartdebugFlag == 1:
                if cbstartdebugFlag == 0:
                    subprocess.getstatusoutput([adbpath,"-s",device,"shell", "screencap", "-p", "/sdcard/" + screenFileName])
                    subprocess.getstatusoutput([adbpath,"-s",device,"pull", "/sdcard/" + screenFileName, os.getcwd()])
                img_target_rgb = cv.imdecode(np.fromfile(screenFileName,dtype=np.uint8),-1)
                img_target_gray = cv.cvtColor(img_target_rgb, cv.COLOR_BGR2GRAY)
                ui.image2Update()
                # 直方图信息
                # img_hist = cv.calcHist([img_target_gray],[0],None,[256],[0,256])
                # cv.normalize(img_hist, img_hist,0,255*0.9,cv.NORM_MINMAX)
                # img_rgb_hist = cv.calcHist([img_target_rgb],[0],None,[256],[0,256])
                # cv.normalize(img_rgb_hist, img_rgb_hist,0,255*0.9,cv.NORM_MINMAX)

                items=ui.tree2.get_children()
                for index,item in enumerate(items):
                    row = ui.tree2.item(item,"values")
                    if row[1] == '√' :
                        area = re.findall(r"([0-9 ]*),([0-9 ]*)\|([0-9 ]*),([0-9 ]*)", row[5])
                        if len(area) == 1:
                            lefttop =  (int(int(area[0][0]) / 100 * img_target_gray.shape[1] ),int(int(area[0][1])/ 100 * img_target_gray.shape[0]) )
                            rightbottm =  (int(int(area[0][2]) / 100 * img_target_gray.shape[1] ),int(int(area[0][3])/ 100 * img_target_gray.shape[0]) )
                            img_targent = img_target_gray[lefttop[1]:rightbottm[1], lefttop[0]:rightbottm[0]]
                            kps_target, features = sift.detectAndCompute(img_targent ,None)
                        else:
                            img_targent = img_target_gray
                            kps_target, features = sift.detectAndCompute(img_targent ,None)

                        model = config.templateConfigs[row[2]].models[row[3]]
                        success,good,matchesMask,dst  = model.matchSift(flann,img_targent,features,kps_target)
                        if success:
                            cv.polylines(img_targent,[np.int32(dst)],True,0,2, cv.LINE_AA)
                            # 点击
                            if cbstartdebugFlag == 0:
                                if len(area) == 1:
                                    x = int((dst[0][0][0] + dst[2][0][0])//2) + lefttop[0]
                                    y = int((dst[0][0][1] + dst[2][0][1])//2) + lefttop[1]

                                    subprocess.getstatusoutput([adbpath,"-s",device,"shell","input","tap",str(x) ,str(y)])
                                else:
                                    subprocess.getstatusoutput([adbpath,"-s",device,"shell","input","tap",str((dst[0][0][0] + dst[2][0][0])//2) ,str((dst[0][0][1] + dst[2][0][1])//2)])
                            
                            
                            draw_params = dict(matchColor=(0,255,0), singlePointColor=None,matchesMask=matchesMask, flags=2)
                            successResult = cv.drawMatches(model.image,model.kps,img_targent,kps_target,good,None,**draw_params)
                            cv.imwrite("successResult.png",successResult)
                            # 增加次数
                            count = int(row[7])
                            ui.tree2.set(item, column='count', value=count+1)
                            # 更新图片
                            ui.image1Update()

                            # 更新匹配信息
                            ui.configs['pptime'].set( time.strftime("%H:%M:%S", time.localtime()) )

                            ui.configs['ppname'].set( os.path.splitext(os.path.basename(row[3]))[0])
                            
                            sleepTime = int(ui.configs['entrysuccessrate'].get())
                            break



th = threading.Thread(target=scan)
th.setDaemon(True)
th.start()

ui.image1Update()
ui.image2Update()
ui.refreshDevices()

ui.start()



