from enum import auto
import os
import sys
import json
from tkinter.constants import CENTER, EW, LEFT, NS, NSEW, NW, W
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
            'cbstart' : tk.IntVar(value=0)
            
        }

        frameLeft = tk.Frame(window,width=250)
        frameCenter = tk.Frame(window,height=100,width=600)
        frameCenter2 = tk.Frame(window,width=600)
        frameCenter3 = tk.Frame(window,height=250,width=600)
        frameRight = tk.Frame(window,width=200)

        frameRight.grid(row=0, column=2,rowspan=3,sticky=NSEW)
        frameLeft.grid(row=0, column=0,rowspan=3,sticky=NS)
        frameCenter.grid(row=0, column=1,sticky=EW)
        frameCenter2.grid(row=1, column=1,sticky=NSEW)
        frameCenter3.grid(row=2, column=1,sticky=EW)

        window.rowconfigure(1,weight=1)
        window.columnconfigure(2,weight=1)
        frameCenter.columnconfigure(0,weight=1)
        frameCenter.rowconfigure(0,weight=1)
        frameCenter2.columnconfigure(0,weight=1)
        frameCenter2.rowconfigure(0,weight=1)
        frameCenter3.columnconfigure(0,weight=1)
        frameCenter3.rowconfigure(0,weight=1)
        frameRight.columnconfigure(0,weight=1)
        frameRight.rowconfigure(0,weight=1)



        center_frame = tk.Frame(frameCenter)
        center_frame.grid(sticky=NSEW) 

        center_frame_top = tk.Frame(center_frame)
        center_frame_top.grid(sticky=NSEW) 

        labelframeCenter = ttk.LabelFrame(center_frame, text='安卓设备')
        labelframeCenter.grid(row=1, padx=5, pady=5,columnspan=10, sticky=NSEW) 
        labelframeCenter.columnconfigure(0,weight=1)

        right_labelframe = ttk.LabelFrame(frameRight, text='截图')
        right_labelframe.grid( padx=5, pady=5,sticky=NSEW) 
        # # labelframe1.propagate(0) # 使组件大小不变，此时width才起作用
        right_labelframe.columnconfigure(0,weight=1)
        right_labelframe.rowconfigure(0,weight=1)

        center2_labelframe = ttk.LabelFrame(frameCenter2, text='素材列表')
        center2_labelframe.grid( padx=5, pady=5,sticky=NSEW) 
        center2_labelframe.columnconfigure(0,weight=1)
        center2_labelframe.rowconfigure(0,weight=1)

        center3_labelframe = ttk.LabelFrame(frameCenter3, text='操作')
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
        # frameCenter2.configure(bg='#300000')
        # frameCenter3.configure(bg='#500000')
        # frameRight.configure(bg='#700000')


        # frameLeft.columnconfigure(0,j)


        # tk.Checkbutton(frameLeft, justify=LEFT,text='adb启动', variable=cb1,width=15).grid(sticky=NW)
        ttk.Checkbutton(frameLeft,  text='启动adb', variable = self.configs["cbstart"],width=15).grid(padx=5, pady=5)
        # ttk.Checkbutton(frameLeft,  text='启动3', variable=cb1,width=15).grid(sticky=NW)
        # tk.Checkbutton(frameLeft, justify=tk.LEFT, text='启动2', variable=cb1).grid()


        # right
        img = ttk.Label(right_labelframe, image = None)
        img.grid(padx=5, pady=5)

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


        ttk.Label(center3_labelframe, text="adb路径:").grid(column=0, row=0,sticky=W,padx=2, pady=2)
        ttk.Entry(center3_labelframe, width=25, textvariable=self.configs['entryadb']).grid(column=1, row=0,sticky=W,padx=2, pady=2)
        ttk.Label(center3_labelframe, text="scrcpy路径:").grid(column=0, row=1,sticky=W,padx=2, pady=2)
        ttk.Entry(center3_labelframe, width=25, textvariable=self.configs['entryscrcpy']).grid(column=1, row=1,sticky=W,padx=2, pady=2)

        # center3_button = ttk.Button(center3_labelframe,  text = '选择',  width = 10, command = None)
        # center3_button.grid(padx=5, pady=5,sticky=W)

    

        tree2 = ttk.Treeview(center2_labelframe,  columns=('index','status','name','path','area','count'),show='headings')
        tree2.grid(padx=5, pady=5,sticky=NSEW)
        scroll = tk.Scrollbar(center2_labelframe)
        scroll.grid(column=1,row=0,sticky=NSEW)
        scroll.config(command=tree2.yview)
        tree2.config(yscrollcommand = scroll.set)
        tree2.column("index", width=35,anchor=CENTER) 
        tree2.column("status", width=35,anchor=CENTER) 
        tree2.column("name", width=100,anchor=CENTER) 
        tree2.column("path", width=300)
        tree2.column("area", width=80,anchor=CENTER)
        tree2.column("count", width=80,anchor=CENTER)
        tree2.heading("status", text="启用")
        tree2.heading("name", text="名称")
        tree2.heading("path", text="路径")
        tree2.heading("area", text="识别范围")
        tree2.heading("count", text="识别次数")

        self.img = img
        self.tree1 = tree1
        self.tree2 = tree2
        self.frameCenter2 = frameCenter2
        self.center2_labelframe = center2_labelframe
        self.window = window

    def start(self):
        self.window.mainloop()




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
        screenFileName = "screen.jpg"
        subprocess.getstatusoutput([self.configs['entryadb'].get(),"-s",device,"shell", "screencap", "-p", "/sdcard/"+screenFileName])
        subprocess.getstatusoutput([self.configs['entryadb'].get(),"-s",device,"pull", "/sdcard/"+screenFileName, os.getcwd()])
        img_screen = tk.PhotoImage(file="screen.jpg")
        self.img.config(image=img_screen)  
        self.img.image= img_screen


    def scrcpyDevice(self):
        select=self.tree1.selection()
        if len(select) == 0 :
            tk.messagebox.showwarning('警告','请选中一个设备！')
            return
        device = self.tree1.item(select)['values'][1]
        result = subprocess.Popen([self.configs['entryscrcpy'].get(),"-s",device,"-S"],shell=True)
        print(result)


    def scan(self):
        sleepTime = 3
        while True:
            time.sleep(sleepTime)
            # print('是否启动：【%s】' %str(checkBox11Value.get()))
            sleepTime = 3