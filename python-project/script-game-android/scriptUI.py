from enum import auto
import os
import sys
import json
from tkinter.constants import EW, NS, NSEW
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




class ScriptUI:
    def __init__(self):
        window = tk.Tk()           
        window.title('碧蓝航线&DC脚本(夜神模拟器)')   
        window.geometry('1000x720')

        frameLeft = tk.Frame(window,width=250)
        frameCenter = tk.Frame(window,width=600)
        frameCenter2 = tk.Frame(window,height=250,width=600)
        frameRight = tk.Frame(window,width=200)
        window.rowconfigure(0,weight=1)
        window.columnconfigure(2,weight=1)
        frameCenter.columnconfigure(0,weight=1)
        frameCenter.rowconfigure(0,weight=1)
        # frameRight.pack(side=tk.RIGHT, expand='yes', fill='both')

        frameRight.grid(row=0, column=2,rowspan=2,sticky=NSEW)
        frameLeft.grid(row=0, column=0,rowspan=2,sticky=NS)
        frameCenter.grid(row=0, column=1,padx=1,pady=3,sticky=NSEW)
        frameCenter2.grid(row=1, column=1,padx=1,pady=3)

        # scroll = tk.Scrollbar()
        # scroll.grid(row=0, column=1,padx=1,pady=3,sticky=NSEW)
        # scroll.config(command=frameCenter.yview)
        # frameCenter.config(yscrollcommand = scroll.set)

        frameLeft.configure(bg='#100000')
        frameCenter.configure(bg='#300000')
        frameCenter2.configure(bg='#500000')
        frameRight.configure(bg='#700000')






        self.frameCenter = frameCenter
        self.window = window



    def start(self):
        self.window.mainloop()