import binascii
import struct
import os
import time

# 一个神奇的数学原理，所以，只要用原先压缩过的dat文件的值进行开头的异或，就能知道值了
# DA = A ^ K
# DB = B ^ K
# DA ^ DB = (A ^ K) ^ (B ^ K) = (A ^ B) ^ (K ^ K) = A ^ B
def IsJpgOrPng(dir):
    PNG = 0x89 ^ 0x50
    JPG = 0xff ^ 0xd8
    TRFF = 0x49 ^ 0x49
    BMP = 0x42 ^ 0x4d
    with open(dir,"rb") as f1:
        number = f1.read(2)
        answer = int(number[0]) ^ int(number[1])
        if (answer == PNG):
            # print("%s is a PNG"%dir)
            key = int(number[0]) ^ 0x89
            kind = "png"
        elif (answer == JPG):
            # print("%s is a JPG"%dir)
            key = int(number[0]) ^ 0xff
            kind = "jpg"
        elif (answer == TRFF):
            key = int(number[0]) ^ 0x49
            kind = "trff"
        elif (answer == BMP):
            key = int(number[0]) ^ 0x42
            kind = "bmp"
    return key,kind


def Xor(ciphertext,key):
    answer = []
    for i in ciphertext:
        i = i ^ key
        answer.append(i)
    return answer

def BytToStr(string):
    """此函数的作用就是将读取到的16进制字符流变成正常的字符列表，方便后面的异或"""
    hexs = []
    for s in string:
        hexs.append(s)
    return hexs

def GetTimeStamp():
    # 网上找的，使用当前时间作为文件名
    ct = time.time()
    local_time = time.localtime(ct)
    data_head = time.strftime("%Y%m%d%H%M%S", local_time)
    data_secs = (ct - int(ct)) * 1000
    time_stamp = "%s_%03d" % (data_head, data_secs)
    return time_stamp

def change(file1):
    f1 = open(r'%s'%file1,'rb')
    
    length = os.path.getsize(r'%s'%file1)
    # 动态获取要读取的文件的大小

    key,kind = IsJpgOrPng(file1)
    # 我的秘钥是28

    byte = f1.read(length)
    # 将读取到的内容先全部放入一个变量里面

    ciphertext = BytToStr(byte)

    answer = Xor(ciphertext,key)

    filename = GetTimeStamp()
    
    with open(r"%s.%s" % (filename, kind),"ab+") as fout:
        for i in answer:
            a = struct.pack('B',i)
            fout.write(a)

def RecursiveFileSearch(path):
    if (not os.path.exists(path)):	# 判断文件(文件夹)是否存在
        print(path + "不存在！！")
        return 0
    if (os.path.isfile(path) and path[-3:] == 'dat') :
        change(path)
        print("文件%s已经修改完成"%path)
    elif (os.path.isdir(path)):
        dirlist = os.listdir(path)
        for dir in dirlist:
            dir = path + '\\' + dir
            RecursiveFileSearch(dir)


if __name__ == "__main__":
    path  = input()
    RecursiveFileSearch(path)