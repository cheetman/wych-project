

import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt











class Template:
    def __init__(self, sift, name, filename, siftRatio=0.2):
        self.name = name
        self.image = cv.imdecode(np.fromfile(filename,dtype=np.uint8),-1)
        self.garyImage = cv.cvtColor(self.image, cv.COLOR_BGR2GRAY)
        if self.image is None:
            raise Exception("载入模板图片失败：{0}".format(filename))
        self.kps, self.features = sift.detectAndCompute(self.image, None)
        if len(self.kps) == 0 or self.features is None:
            raise Exception("无法检测到特征点：{0}".format(filename))
        self.siftRatio = siftRatio

    def matchSift(self, flann,targetImg,targetFeatures):
        matches = flann.knnMatch(self.features, targetFeatures, 2)
        good = []
        for m, n in matches:
            if m.distance < n.distance * 0.66:
                good.append(m)
        if len(good) > len(self.kps) * self.siftRatio:
            return True,good
        else:
            print( "Not enough matches are found - " + self.name )
            return False,good

        