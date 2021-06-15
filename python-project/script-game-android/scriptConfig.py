
from tkinter.constants import BOTH, CENTER, EW, NSEW, W
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
    def __init__(self,frame):
        self.templateConfigs = {}
        
        self.frame = frame
        tree = ttk.Treeview(frame,  columns=('index','name','path','count'),show='headings')
        tree.grid(sticky=NSEW)

        scroll = tk.Scrollbar(frame)
        scroll.grid(column=1,row=0,sticky=NSEW)
        scroll.config(command=tree.yview)
        tree.config(yscrollcommand = scroll.set)

        tree.column("index", width=35,anchor=CENTER) 
        tree.column("name", width=100,anchor=CENTER) 
        tree.column("path", width=300)
        tree.column("count", width=80,anchor=CENTER)
        tree.heading("name", text="名称") 
        tree.heading("path", text="路径")
        tree.heading("count", text="识别次数")
        self.tree = tree

    def AddPicConfig(self,name,path,type):
        self.templateConfigs[name] = ScriptConfig.TemplateConfig(name,path,type,self.tree)

    class TemplateConfig:
        def __init__(self,name,path,type,tree):
            self.name = name
            self.path = path
            self.type = type

            files = os.listdir(path)
            for index,file in enumerate(files):
                tmpPath = os.path.join(path, file)
                self.model = scriptModel.Template(sift, name,tmpPath)
                    
                tree.insert("", 'end',  values=(index +1,name, tmpPath))  

    class TemplateType(Enum):
        CLICK = 1
