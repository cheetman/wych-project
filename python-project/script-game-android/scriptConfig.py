
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
    def __init__(self,tree):
        self.templateConfigs = {}
        self.tree = tree


    def AddPicConfig(self,name,path,type):
        self.templateConfigs[name] = ScriptConfig.TemplateConfig(name,path,type,self.tree)

    def scanDevice(self):
        items=self.tree.get_children()
        for index,item in enumerate(items):
            # row = self.tree.item(item)
            self.tree.set(item, column='status', value='1212')



    class TemplateConfig:
        def __init__(self,name,path,type,tree):
            self.name = name
            self.path = path
            self.type = type

            files = os.listdir(path)
            for index,file in enumerate(files):
                tmpPath = os.path.join(path, file)
                self.model = scriptModel.Template(sift, name,tmpPath)
                tree.insert("", 'end',  values=(len(tree.get_children()) +1,'√',  name, tmpPath,'全部',0))  

    class TemplateType(Enum):
        CLICK = 1
