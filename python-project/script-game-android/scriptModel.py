
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


class Template:
    def __init__(self, sift, name, filename, siftRatio=0.2,histThreshold = 0.02):
        self.name = name
        self.image = cv.imdecode(np.fromfile(filename,dtype=np.uint8),-1)
        self.garyImage = cv.cvtColor(self.image, cv.COLOR_BGR2GRAY)
        if self.image is None:
            raise Exception("载入模板图片失败：{0}".format(filename))
        self.kps, self.features = sift.detectAndCompute(self.image, None)
        if len(self.kps) == 0 or self.features is None:
            raise Exception("无法检测到特征点：{0}".format(filename))
        self.siftRatio = siftRatio
        self.garyImageHist = cv.calcHist([self.garyImage],[0],None,[256],[0,256])
        cv.normalize(self.garyImageHist, self.garyImageHist,0,255*0.9,cv.NORM_MINMAX)
        self.imageHist = cv.calcHist([self.image],[0],None,[256],[0,256])
        cv.normalize(self.imageHist, self.imageHist,0,255*0.9,cv.NORM_MINMAX)
        self.histThreshold = histThreshold

    def matchSift(self, flann,targetImg,targetFeatures,kps):
        matches = flann.knnMatch(self.features, targetFeatures, 2)
        good = []
        for m, n in matches:
            if m.distance < n.distance * 0.66:
                good.append(m)
        if len(good) > len(self.kps) * self.siftRatio:
            # 获取关键点的坐标
            src_pts = np.float32([ self.kps[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
            dst_pts = np.float32([ kps[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
            # 计算变换矩阵和MASK
            M, mask = cv.findHomography(src_pts, dst_pts, cv.RANSAC, 5.0)
            if M is None:
                print( "M is None - " + str(self.name) )
                return False,good,None,None
            matchesMask = mask.ravel().tolist()
            h,w = self.garyImage.shape
            # 使用得到的变换矩阵对原图像的四个角进行变换，获得在目标图像上对应的坐标
            pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
            dst = cv.perspectiveTransform(pts,M)
            return True,good,matchesMask,dst
        else:
            print( "Not enough matches are found - " + str(self.name) )
            return False,good,None,None

    def matchHist(self, imgHist):
        match = cv.compareHist(imgHist,self.imageHist,cv.HISTCMP_BHATTACHARYYA)
        if match < self.histThreshold:
            print('直方图匹配-' + str(self.name) + ' - ' + str(match))
            return True
        else:
            print('直方图未匹配-'+ str(self.name) + ' - ' + str(match))
            return False