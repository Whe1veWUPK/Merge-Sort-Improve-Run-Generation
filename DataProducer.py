import numpy as np
import os
"""随机生成的数据的数值范围 -10000-10000"""
MAXNUM = 100000
MINNUM = -100000
"""设置数据文件生成路径"""
path = os.getcwd()

def produceData(size,filename):
    """根据传入的数据规模 以及文件名称 生成给定长度的随机数据 并将数据存入对应的 txt中"""
    randomData=np.random.randint(MINNUM,MAXNUM,(size,1))
    np.savetxt(path+r'\\'+filename+r'.txt',randomData,fmt="%d",delimiter=' ')


inputDataSize = input("Please input the data's size: ")

dataSize = int(inputDataSize)

produceData(dataSize, "Input")

