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
import threading
import scriptModel
from PIL import Image, ImageFilter

def showSiftMatchImage(label_img,template,img_target_gray,kps_target,good,matchesMask):
  draw_params = dict(matchColor=(0,255,0), 
                            singlePointColor=None,
                            matchesMask=matchesMask, 
                            flags=2)
  result = cv.drawMatches(template.image,template.kps,img_target_gray,kps_target,good,None,**draw_params)
  cv.imwrite("result.png",result)
  img_screen = tk.PhotoImage(file="result.png")
  label_img.config(image=img_screen)  
  label_img.image= img_screen


#创建sift检测器
sift=cv.xfeatures2d.SIFT_create()
templates = []
templates.append(scriptModel.Template(sift, 1,"3.大获全胜S.jpg"))
templates.append(scriptModel.Template(sift, 2,"3.大获全胜S2.jpg"))
templates.append(scriptModel.Template(sift, 3,"3.战术碾压A.jpg"))
templates.append(scriptModel.Template(sift, 4,"4.获得道具.jpg"))
templates.append(scriptModel.Template(sift, 5,"6.大获全胜2确定按钮.jpg"))
templates.append(scriptModel.Template(sift, 6,"5.大获全胜2.jpg"))
templates.append(scriptModel.Template(sift, 7,"2.出击图标.jpg"))
templates.append(scriptModel.Template(sift, 8,"1.立刻前往图标.jpg"))


template2 = scriptModel.Template(sift, 8,"8.地图条件.jpg")
templates2 = []
# templates2.append(scriptModel.Template(sift, 1,"8.敌人1.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人2.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人3.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人4.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人5.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人6.jpg"))
# templates2.append(scriptModel.Template(sift, 1,"8.敌人7.jpg"))

files = os.listdir('.\8.敌人目录')
for file in files:
  path = os.path.join('.\8.敌人目录', file)
  templates2.append(scriptModel.Template(sift, 1,path))
  

templates3 = []
templates3.append(scriptModel.Template(sift, 1,"9.海域突进首页.jpg"))
templates3.append(scriptModel.Template(sift, 1,"9.商船护卫首页.jpg"))
templates3.append(scriptModel.Template(sift, 1,"9.斩首行动首页.jpg"))
templates3.append(scriptModel.Template(sift, 1,"9.战术研修首页.jpg"))

templates4 = []
templates4.append(scriptModel.Template(sift, 1,"9.特别演习-埃塞克斯.jpg"))

templateBoss = scriptModel.Template(sift, 8,"boss.jpg")

templatePosition = scriptModel.Template(sift, 8,"7.定位图标.jpg")

#设置Flannde参数
FLANN_INDEX_KDTREE=0
indexParams=dict(algorithm=FLANN_INDEX_KDTREE,trees=5)
searchParams= dict(checks=50)
flann=cv.FlannBasedMatcher(indexParams,searchParams)



def task():
  if checkBox11Value.get() == 1:
      # 1.读入截图
    os.system("adb -s 127.0.0.1:62001 shell screencap -p /sdcard/screen.jpg")
    os.system("adb -s 127.0.0.1:62001 pull /sdcard/screen.jpg " + os.getcwd())
    img_target_rgb = cv.imdecode(np.fromfile("screen.jpg",dtype=np.uint8),-1)
    img_target_gray = cv.cvtColor(img_target_rgb, cv.COLOR_BGR2GRAY)
    kps_target, features = sift.detectAndCompute(img_target_gray ,None)
    img_hist = cv.calcHist([img_target_gray],[0],None,[256],[0,256])
    cv.normalize(img_hist, img_hist,0,255*0.9,cv.NORM_MINMAX)
    img_rgb_hist = cv.calcHist([img_target_rgb],[0],None,[256],[0,256])
    cv.normalize(img_rgb_hist, img_rgb_hist,0,255*0.9,cv.NORM_MINMAX)

    # 判断是否在战斗界面
    success,good,matchesMask,dst = template2.matchSift(flann,img_target_gray,features,kps_target)
    if success:
      # 1.在战斗界面

      # 1.1特殊地图
      if checkBox5Value.get() == 1:
        # 优先boss
        if checkBox4Value.get() == 1:
          success,good,matchesMask,dst = templateBoss.matchSift(flann,img_target_gray,features,kps_target)
          if success:
            cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
            os.system( entry1Value.get() + " shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
            showSiftMatchImage(label_img,templateBoss,img_target_gray,kps_target,good,matchesMask)
            return

        # 普通敌人
        for template in templates2:
          time.sleep(0.01)
          success,good,matchesMask,dst = template.matchSift(flann,img_target_gray,features,kps_target)
          if success:
            cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
            os.system( entry1Value.get() + " shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
            showSiftMatchImage(label_img,template,img_target_gray,kps_target,good,matchesMask)
      else:

        # 优先boss
        if checkBox4Value.get() == 1:
          success,good,matchesMask,dst = templateBoss.matchSift(flann,img_target_gray,features,kps_target)
          if success:
            cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
            os.system( entry1Value.get() + " shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
            showSiftMatchImage(label_img,templateBoss,img_target_gray,kps_target,good,matchesMask)
            return

        # 普通敌人
        for template in templates2:
          time.sleep(0.01)
          success,good,matchesMask,dst = template.matchSift(flann,img_target_gray,features,kps_target)
          if success:
            cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
            os.system( entry1Value.get() + " shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
            showSiftMatchImage(label_img,template,img_target_gray,kps_target,good,matchesMask)
            return
    
    
    else:
      # 2.不在战斗界面
      if checkBox3Value.get() == 1:
        for template in templates3:
          success = template.matchHist(img_rgb_hist)
          if success:
            os.system( entry1Value.get() + " shell input tap "+ entry1Value.get().split(',')[0] +" " + entry1Value.get().split(',')[1])


      # 特别演习
      if checkBox6Value.get() == 1:
        for template in templates4:
          success = template.matchHist(img_rgb_hist)
          if success:
            os.system( entry1Value.get() + " shell input tap "+ str(857) +" " + str(216))
      if checkBox7Value.get() == 1:
        for template in templates4:
          success = template.matchHist(img_rgb_hist)
          if success:
            os.system( entry1Value.get() + " shell input tap "+ str(826) +" " + str(302))
      if checkBox8Value.get() == 1:
        for template in templates4:
          success = template.matchHist(img_rgb_hist)
          if success:
            os.system( entry1Value.get() + " shell input tap "+ str(847) +" " + str(389))

      for template in templates:
        time.sleep(0.01)
        success,good,matchesMask,dst = template.matchSift(flann,img_target_gray,features,kps_target)
        if success:
          cv.polylines(img_target_gray,[np.int32(dst)],True,0,2, cv.LINE_AA)
          os.system( "adb -s 127.0.0.1:62001 shell input tap "+ str((dst[0][0][0] + dst[2][0][0])//2) +" " + str((dst[0][0][1] + dst[2][0][1])//2))
          showSiftMatchImage(label_img,template,img_target_gray,kps_target,good,matchesMask)




def scanGameAdb():
  sleepTime = 4
  while True:
    time.sleep(sleepTime)
    print('是否启动：【%s】' %str(checkBox11Value.get()))
    task()
    sleepTime = 3



window = tk.Tk()            #主窗口
window.title('碧蓝航线脚本(夜神模拟器)')   #窗口标题
window.geometry('700x400')  #窗口尺寸

menubar = tk.Menu(window)
filemenu = tk.Menu(menubar, tearoff=0)
menubar.add_cascade(label='File', menu=filemenu)
filemenu.add_command(label='Exit', command=window.quit)
window.config(menu=menubar)

frameLeft = tk.Frame(window)
frameRight = tk.Frame(window)
frameBottom = tk.Frame(window)

frameLeft.pack(side=tk.LEFT, fill='y', expand='no')
frameRight.pack(side=tk.TOP, fill='x')
frameBottom.pack(side=tk.RIGHT, expand='yes', fill='both')

checkBox1Value = tk.IntVar()
checkBox11Value = tk.IntVar()
checkBox2Value = tk.IntVar()
checkBox3Value = tk.IntVar()
checkBox4Value = tk.IntVar()
checkBox5Value = tk.IntVar()
checkBox6Value = tk.IntVar()
checkBox7Value = tk.IntVar()
checkBox8Value = tk.IntVar()
entry1Value = tk.StringVar(value='adb -s 127.0.0.1:62001')
entry2Value = tk.StringVar(value='500,180')
entry3Value = tk.StringVar(value='500,180')

c1 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='开始执行', variable=checkBox1Value).pack()
c11 = tk.Checkbutton(frameLeft,justify=tk.LEFT,  text='开始执行(adb)', variable=checkBox11Value).pack()
c2 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='自动寻路', variable=checkBox2Value).pack()
c3 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='任务界面', variable=checkBox3Value).pack()
c4 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='优先BOSS', variable=checkBox4Value).pack()
c5 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特殊地图', variable=checkBox5Value).pack()
c6 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯', variable=checkBox6Value).pack()
c7 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯(普通)', variable=checkBox7Value).pack()
c8 = tk.Checkbutton(frameLeft, justify=tk.LEFT, text='特演-埃塞克斯(简单)', variable=checkBox8Value).pack()


tk.Label(frameBottom,text="命令地址:").grid(row=0,column=0)
tk.Entry(frameBottom,textvariable = entry1Value).grid(row=0,column=1,columnspan=4,sticky=tk.EW)

tk.Label(frameBottom,text="任务1坐标:").grid(row=1,column=0)
tk.Label(frameBottom,text="任务2坐标:").grid(row=1,column=2)

tk.Entry(frameBottom,textvariable = entry2Value).grid(row=1,column=1)
tk.Entry(frameBottom,textvariable = entry3Value).grid(row=1,column=3)

# e1.grid(row=0, column=0, padx=10, pady=10, ipadx=10, ipady=10)
# e2.grid(row=0, column=1, padx=10, pady=10, ipadx=10, ipady=10)
# e3.grid(row=1, column=0, padx=10, pady=10, ipadx=10, ipady=10)


img_screen = tk.PhotoImage(file="screen.jpg")
label_img = tk.Label(frameRight, image = img_screen)
label_img.pack()




th2 = threading.Thread(target=scanGameAdb)
th2.setDaemon(True)
th2.start()


window.mainloop()    