from enum import auto
import os
import sys
import json
from tkinter.constants import CENTER, EW, HORIZONTAL, LEFT, NS, NSEW, NW, VERTICAL, W
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
import re
import subprocess
from PIL import Image, ImageFilter

import tkinter as tk
from tkinter import image_types, ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import Spinbox
from tkinter import messagebox as mBox




class ScriptUI:
    def __init__(self):
        window = tk.Tk()           
        window.title('碧蓝航线脚本')   
        window.geometry('1000x720')

        self.configs = {
            'entryadb' : tk.StringVar(value='adb'),
            'entryscrcpy' : tk.StringVar(value='scrcpy-console.bat'),
            'entryrate' : tk.StringVar(value='5'),
            'entrysuccessrate' : tk.StringVar(value='2'),
            'entrypiclength' : tk.StringVar(value='1000'),


            'cbstart' : tk.IntVar(value=0),
            'cbstartdebug' : tk.IntVar(value=0),
            'cbppalert' : tk.IntVar(value=0),
            
            'pptime' : tk.StringVar(value='无'),
            'ppname' : tk.StringVar(value='无'),
            
        }

        frameLeft = tk.Frame(window,width=250)
        frameCenter = tk.Frame(window,height=100,width=600)
        frameCenter2 = tk.Frame(window,height=100,width=600)
        frameCenter3 = tk.Frame(window,width=600)
        frameCenter4 = tk.Frame(window,height=250,width=600)
        frameRight = tk.Frame(window,width=200)

        frameRight.grid(row=0, column=2,rowspan=4,sticky=NSEW)
        frameLeft.grid(row=0, column=0,rowspan=4,sticky=NS)
        frameCenter.grid(row=0, column=1,sticky=EW)
        frameCenter2.grid(row=1, column=1,sticky=EW)
        frameCenter3.grid(row=2, column=1,sticky=NSEW)
        frameCenter4.grid(row=3, column=1,sticky=EW)

        window.rowconfigure(2,weight=1)
        window.columnconfigure(2,weight=1)
        frameCenter.columnconfigure(0,weight=1)
        frameCenter.rowconfigure(0,weight=1)
        frameCenter3.columnconfigure(0,weight=1)
        frameCenter3.rowconfigure(0,weight=1)
        frameCenter4.columnconfigure(0,weight=1)
        frameCenter4.rowconfigure(0,weight=1)
        frameRight.columnconfigure(0,weight=1)
        frameRight.rowconfigure(0,weight=1)
        frameRight.rowconfigure(1,weight=1)



        center_frame = tk.Frame(frameCenter)
        center_frame.grid(sticky=NSEW) 

        center_frame_top = tk.Frame(center_frame)
        center_frame_top.grid(sticky=NSEW) 

        labelframeCenter = ttk.LabelFrame(center_frame, text='安卓设备')
        labelframeCenter.grid(row=1, padx=5, pady=5,columnspan=1, sticky=NSEW) 
        labelframeCenter.columnconfigure(0,weight=1)

        
        labelframeCenter3 = ttk.LabelFrame(center_frame, text='匹配信息')
        labelframeCenter3.grid(row=1,column=1, padx=5, pady=5,columnspan=1, sticky=NSEW) 
        labelframeCenter3.columnconfigure(0,weight=1)

        right_labelframe = ttk.LabelFrame(frameRight, text='匹配成功')
        right_labelframe.grid( padx=5, pady=5,sticky=NSEW) 
        # # labelframe1.propagate(0) # 使组件大小不变，此时width才起作用
        right_labelframe.columnconfigure(0,weight=1)
        right_labelframe.rowconfigure(0,weight=1)

        
        right_labelframe2 = ttk.LabelFrame(frameRight, text='截图')
        right_labelframe2.grid( padx=5, pady=5,sticky=NSEW)
        right_labelframe2.columnconfigure(0,weight=1)
        right_labelframe2.rowconfigure(0,weight=1)

        center2_labelframe = ttk.LabelFrame(frameCenter3, text='素材列表')
        center2_labelframe.grid( padx=5, pady=5,sticky=NSEW) 
        center2_labelframe.columnconfigure(0,weight=1)
        center2_labelframe.rowconfigure(0,weight=1)

        center3_labelframe = ttk.LabelFrame(frameCenter4, text='设置')
        center3_labelframe.grid( padx=5, pady=5,sticky=NSEW) 
        # center3_labelframe.columnconfigure(0,weight=1)
        # center3_labelframe.rowconfigure(0,weight=1)

        # frameRight.pack(side=tk.RIGHT, expand='yes', fill='both')


        # scroll = tk.Scrollbar() 
        # scroll.grid(row=0, column=1,padx=1,pady=3,sticky=NSEW)
        # scroll.config(command=frameCenter.yview)
        # frameCenter.config(yscrollcommand = scroll.set)

        # frameLeft.configure(bg='#100000')
        # frameCenter.configure(bg='#300000')
        # frameCenter3.configure(bg='#300000')
        # frameCenter4.configure(bg='#500000')
        # frameRight.configure(bg='#700000')


        # frameLeft.columnconfigure(0,j)


        # tk.Checkbutton(frameLeft, justify=LEFT,text='adb启动', variable=cb1,width=15).grid(sticky=NW)
        ttk.Checkbutton(frameLeft,  text='启动adb', variable = self.configs["cbstart"],width=15).grid(padx=5, pady=1)
        ttk.Checkbutton(frameLeft,  text='启动adb(调试)', variable = self.configs["cbstartdebug"],width=15).grid(padx=5, pady=1)
        ttk.Checkbutton(frameLeft,  text='长时间未匹配提醒', variable = self.configs["cbppalert"],width=15).grid(padx=5, pady=1)
        # ttk.Checkbutton(frameLeft,  text='启动3', variable=cb1,width=15).grid(sticky=NW)
        # tk.Checkbutton(frameLeft, justify=tk.LEFT, text='启动2', variable=cb1).grid()


        #center 
        
        ttk.Button(center_frame_top,  text = '刷新',  width = 10, command = self.refreshDevices).grid(padx=5, pady=5,sticky=W)
        ttk.Button(center_frame_top,  text = '截图',  width = 10, command = self.screenCap).grid(row=0,column=1, padx=5, pady=5,sticky=W)
        ttk.Button(center_frame_top,  text = 'scrcpy投屏',  width = 10, command = self.scrcpyDevice).grid(row=0,column=2, padx=5, pady=5,sticky=W)



        tree1 = ttk.Treeview(labelframeCenter, height=3,  columns=('index','device','status'),show='headings')
        tree1.grid(padx=5, pady=5,sticky=EW)
        tree1.column("index", width=35,anchor=CENTER) 
        tree1.column("device", width=150,anchor=CENTER) 
        tree1.column("status", width=100,anchor=CENTER)
        tree1.heading("device", text="设备") 
        tree1.heading("status", text="状态")

        ttk.Label(labelframeCenter3, text="时间:").grid(column=0, row=0,sticky=W,padx=2, pady=1)
        ttk.Label(labelframeCenter3, textvariable=self.configs['pptime']).grid(column=1, row=0,sticky=W,padx=2, pady=1)
        ttk.Label(labelframeCenter3, text="图片:").grid(column=0, row=1,sticky=W,padx=2, pady=1)
        ttk.Label(labelframeCenter3, textvariable=self.configs['ppname']).grid(column=1, row=1,sticky=W,padx=2, pady=1)


        ttk.Button(frameCenter2,  text = '⬆',   width = 3,command = self.treeMoveUp).grid(padx=5, pady=5,sticky=W)
        ttk.Button(frameCenter2,  text = '⬇',  width = 3, command = self.treeMoveDown).grid(row=0,column=1, padx=5, pady=5,sticky=W)
        ttk.Button(frameCenter2,  text = '刷新列表',  command = self.refreshFile).grid(row=0,column=2,padx=5, pady=5,sticky=W)
        ttk.Button(frameCenter2,  text = '保存配置',  command = self.treeSave).grid(row=0,column=3,padx=5, pady=5,sticky=W)
        ttk.Button(frameCenter2,  text = '测试：显示检测范围、点击坐标',  command = self.signArea).grid(row=0,column=4,padx=5, pady=5,sticky=W)

        # right
        img2 = ttk.Label(right_labelframe2, image = None)
        img2.grid(padx=5, pady=5)

    
        img = ttk.Label(right_labelframe, image = None)
        img.grid(padx=5, pady=5)


        ttk.Label(center3_labelframe, text="adb路径:").grid(column=0, row=0,sticky=W,padx=2, pady=1)
        ttk.Entry(center3_labelframe, width=25, textvariable=self.configs['entryadb']).grid(column=1, row=0,sticky=W,padx=2, pady=1)
        ttk.Label(center3_labelframe, text="scrcpy路径:").grid(column=0, row=1,sticky=W,padx=2, pady=1)
        ttk.Entry(center3_labelframe, width=25, textvariable=self.configs['entryscrcpy']).grid(column=1, row=1,sticky=W,padx=2, pady=1)
        ttk.Label(center3_labelframe, text="扫描频率(秒)").grid(column=2, row=0,sticky=W,padx=2, pady=1)
        ttk.Entry(center3_labelframe, width=10, textvariable=self.configs['entryrate']).grid(column=3, row=0,sticky=W,padx=2, pady=1)
        ttk.Label(center3_labelframe, text="扫描成功等待(秒)").grid(column=2, row=1,sticky=W,padx=2, pady=1)
        ttk.Entry(center3_labelframe, width=10, textvariable=self.configs['entrysuccessrate']).grid(column=3, row=1,sticky=W,padx=2, pady=1)
        ttk.Label(center3_labelframe, text="图片长边(500~1500)").grid(column=4, row=0,sticky=W,padx=2, pady=1)
        ttk.Entry(center3_labelframe, width=10, textvariable=self.configs['entrypiclength']).grid(column=5, row=0,sticky=W,padx=2, pady=1)

        # center3_button = ttk.Button(center3_labelframe,  text = '选择',  width = 10, command = None)
        # center3_button.grid(padx=5, pady=5,sticky=W)

    

        tree2 = ttk.Treeview(center2_labelframe,  columns=('index','status','name','path','type','area','tap','count','countLimit'),show='headings')
        tree2.grid(padx=5, pady=5,sticky=NSEW)

        scroll = tk.Scrollbar(center2_labelframe,orient=VERTICAL)
        scroll.grid(column=1,row=0,sticky=NSEW)
        scroll.config(command=tree2.yview)
        tree2.config(yscrollcommand = scroll.set)

        scrollx = tk.Scrollbar(center2_labelframe,orient=HORIZONTAL)
        scrollx.grid(column=0,row=1,sticky=NSEW)
        scrollx.config(command=tree2.xview)
        tree2.config(xscrollcommand = scrollx.set)

        tree2.column("index", width=35,anchor=CENTER) 
        tree2.column("status", width=35,anchor=CENTER) 
        tree2.column("name", width=90,anchor=CENTER) 
        tree2.column("path", width=260)
        tree2.column("type", width=60,anchor=CENTER)
        tree2.column("area", width=90,anchor=CENTER)
        tree2.column("tap", width=70,anchor=CENTER)
        tree2.column("count", width=60,anchor=CENTER)
        tree2.column("countLimit", width=60,anchor=CENTER)
        tree2.heading("status", text="启用")
        tree2.heading("name", text="名称")
        tree2.heading("path", text="路径")
        tree2.heading("type", text="检测方法")
        tree2.heading("area", text="识别范围")
        tree2.heading("tap", text="点击坐标")
        tree2.heading("count", text="识别次数")
        tree2.heading("countLimit", text="次数限制")
        tree2.bind('<Double-1>', self.gridRightClick)


        self.templateConfig = {}
        self.img2 = img2
        self.img = img
        self.tree1 = tree1
        self.tree2 = tree2
        self.frameCenter3 = frameCenter3
        self.center2_labelframe = center2_labelframe
        self.window = window

    def start(self):
        self.window.mainloop()



    def gridRightClick(self,event):
        for item in self.tree2.selection():
            item_text = self.tree2.item(item,"values")
            if item_text[1] == '√' :
                self.tree2.set(item, column='status', value='')
            else :
                self.tree2.set(item, column='status', value='√')



    def refreshDevices(self):
        items=self.tree1.get_children()
        for item in items:
            self.tree1.delete(item)
        result = subprocess.getstatusoutput([self.configs['entryadb'].get(),"devices"])
        for index,item in  enumerate(filter(lambda x:x.find('\t') > 0 , result[1].split('\n'))):
            self.tree1.insert("", 'end',  values=(index +1,item.split('\t')[0], item.split('\t')[1]))  
        print(result)

    def screenCap(self):
        select=self.tree1.selection()
        if len(select) == 0 :
            tk.messagebox.showwarning('警告','请选中一个设备！')
            return
        device = self.tree1.item(select)['values'][1]
        screenFileName = "screen.png"
        subprocess.getstatusoutput([self.configs['entryadb'].get(),"-s",device,"shell", "screencap", "-p", "/sdcard/"+screenFileName])
        subprocess.getstatusoutput([self.configs['entryadb'].get(),"-s",device,"pull", "/sdcard/"+screenFileName, os.getcwd()])
        self.image2Update()


    def image1Update(self,file = "successResult.png"):
        self.imageUpdate(file,self.img)

    def image2Update(self,file = 'screen.png'):
        self.imageUpdate(file,self.img2)

    def imageUpdate(self,file ,image):
        im1 = cv.imread(file)
        if im1 is not None:
            maxValue = max(im1.shape[0],im1.shape[1])
            minValue = min(im1.shape[0],im1.shape[1])
            entrypiclength = int(self.configs['entrypiclength'].get())
            if maxValue > entrypiclength :
                newMaxValue = entrypiclength
                radio = maxValue/newMaxValue
                # radio = maxValue/minValue
                newMinValue = int(minValue /radio)
                if im1.shape[0] > im1.shape[1]:
                    im2 = cv.resize(im1, (newMinValue,newMaxValue), interpolation=cv.INTER_CUBIC)
                else :
                    im2 = cv.resize(im1, (newMaxValue,newMinValue), interpolation=cv.INTER_CUBIC)
                cv.imwrite('screen2.png', im2)
            else:
                cv.imwrite('screen2.png', im1)

            img_screen = tk.PhotoImage(file="screen2.png")
            image.config(image=img_screen)  
            image.image= img_screen

    def refreshFile(self):
        self.templateConfig.RefreshPicConfig()
        


    def signArea(self):
        
        select=self.tree2.selection()
        if len(select) == 0 :
            tk.messagebox.showwarning('警告','请选中一条记录！')
            return
        area = self.tree2.item(select,'values')[5]
        tap =  self.tree2.item(select,'values')[6]
        if area == "全部" and tap == "默认" :
            tk.messagebox.showwarning('警告','请选中正确记录！')
            return

        image = cv.imread('screen.png')
        if area != "全部":
            area2 = re.findall(r"([0-9 ]*),([0-9 ]*)\|([0-9 ]*),([0-9 ]*)", area)
            lefttop =  (int(int(area2[0][0]) / 100 * image.shape[1] ),int(int(area2[0][1])/ 100 * image.shape[0]) )
            rightbottm =  (int(int(area2[0][2]) / 100 * image.shape[1] ),int(int(area2[0][3])/ 100 * image.shape[0]) )
            cv.rectangle(image,lefttop,  rightbottm, (0, 0, 255), 5)

        if tap != "默认":
            tap2 = re.findall(r"([0-9 ]*),([0-9 ]*)", tap)
            lefttop =  (int(int(tap2[0][0]) / 100 * image.shape[1] -50) ,int(int(tap2[0][1])/ 100 * image.shape[0] -50))
            rightbottm =  (int(int(tap2[0][0]) / 100 * image.shape[1]+ 50 ),int(int(tap2[0][1])/ 100 * image.shape[0] + 50 ))
            cv.rectangle(image,lefttop,  rightbottm, (0, 255, 0), 5)

                
        cv.imwrite('screenArea.png', image)

        self.image2Update('screenArea.png')


    def getDevice(self):
        select=self.tree1.selection()
        if len(select) == 0 :
            return None
        return self.tree1.item(select)['values'][1]

    def scrcpyDevice(self):
        select=self.tree1.selection()
        if len(select) == 0 :
            tk.messagebox.showwarning('警告','请选中一个设备！')
            return
        device = self.tree1.item(select)['values'][1]
        result = subprocess.Popen([self.configs['entryscrcpy'].get(),"-s",device,"-S"],shell=True)
        print(result)


    def treeMoveUp(self):
        sleepTime = 3
        
    def treeMoveDown(self):
        sleepTime = 3

    def treeSave(self):
        sleepTime = 3
        