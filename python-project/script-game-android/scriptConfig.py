import re
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

    def scanDevice(self,success):
        items=self.tree.get_children()
        for index,item in enumerate(items):
            row = self.tree.item(item,"values")
            if row[1] == '√' :
                model = self.templateConfigs[row[2]].models[row[3]]



    class TemplateConfig:
        def __init__(self,name,path,type,tree):
            self.name = name
            self.path = path
            self.type = type
            self.models = {}

            files = os.listdir(path)
            for index,file in enumerate(files):
                tmpPath = os.path.join(path, file)
                self.models[tmpPath] = scriptModel.Template(sift, name,tmpPath)

                tmpName = os.path.basename(tmpPath)
                
                area = '全部'
                area2 = re.findall(r"\[\(([0-9 ]*),([0-9 ]*)\),\(([0-9 ]*),([0-9 ]*)\)\]", tmpName)
                if len(area2) > 0:
                    area = '{0},{1} | {2},{3} '.format(area2[0][0], area2[0][1],area2[0][2],area2[0][3])
                tap = '默认'
                tap2 = re.findall(r"\{([0-9 ]*),([0-9 ]*)\}", tmpName)
                if len(tap2) > 0:
                    tap = '{0},{1} '.format(tap2[0][0], tap2[0][1])
                
                type = 'sfit'
                tree.insert("", 'end',  values=(len(tree.get_children()) +1,'',  name, tmpPath,type,area,tap,0))  

    class TemplateType(Enum):
        CLICK = 1
