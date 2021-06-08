
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
from enum import Enum
import os
import scriptModel


#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()

class ScriptConfig:
    def __init__(self):
        self.templateConfigs = {}

    def AddPicConfig(self,name,path,type):
        self.templateConfigs[name] = ScriptConfig.TemplateConfig(name,path,type)

    class TemplateConfig:
        def __init__(self,name,path,type):
            self.name = name
            self.path = path
            self.type = type
            files = os.listdir(path)
            for file in files:
                path = os.path.join(path, file)
                self.model = scriptModel.Template(sift, name,path)

    class TemplateType(Enum):
        CLICK = 1
