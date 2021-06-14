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
        window.geometry('700x400')

        frameRight = tk.Frame(window)
        frameRight.pack(side=tk.RIGHT, expand='yes', fill='both')







        self.frameRight = frameRight
        self.window = window



    def start(self):
        self.window.mainloop()