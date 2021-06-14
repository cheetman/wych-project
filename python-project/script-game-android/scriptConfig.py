
from tkinter.constants import CENTER
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
from enum import Enum
import os
import scriptModel

import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import Spinbox
from tkinter import messagebox as mBox

#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()

class ScriptConfig:
    def __init__(self):
        self.templateConfigs = {}

    def AddPicConfig(self,name,path,type,frame):
        self.templateConfigs[name] = ScriptConfig.TemplateConfig(name,path,type,frame)

    class TemplateConfig:
        def __init__(self,name,path,type,frame):
            self.name = name
            self.path = path
            self.type = type
            if frame is not None:
                self.frame = frame
                tree = ttk.Treeview(frame, columns=('index','name','path','count'),show='headings')
                tree.grid(row=0,column=0,columnspan=1)

                tree.column("index", width=35,anchor=CENTER) 
                tree.column("name", width=100,anchor=CENTER) 
                tree.column("path", width=300)
                tree.column("count", width=80,anchor=CENTER)
                tree.heading("name", text="名称") 
                tree.heading("path", text="路径")
                tree.heading("count", text="识别次数")

            files = os.listdir(path)
            for index,file in enumerate(files):
                tmpPath = os.path.join(path, file)
                self.model = scriptModel.Template(sift, name,tmpPath)
                    
                tree.insert("", 'end',  values=(index +1,name, tmpPath))  

    class TemplateType(Enum):
        CLICK = 1
