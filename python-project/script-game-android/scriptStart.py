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


# img_screen = tk.PhotoImage(file="screen.jpg")
# ui.img.config(image=img_screen)  
# ui.img.image= img_screen



def scan():
    sleepTime = 3
    while True:
        time.sleep(sleepTime)

        if ui.configs['cbstart'].get() == 1 :
            print('启动扫描......')
            config.scanDevice()


        sleepTime = 3


th = threading.Thread(target=scan)
th.setDaemon(True)
th.start()



ui.start()



