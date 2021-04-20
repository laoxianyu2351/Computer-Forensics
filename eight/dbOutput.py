	# -*- coding: utf-8 -*-

import math
import time
import sys

def get_time_stamp():
    # 网上找的，使用当前时间作为文件名
    ct = time.time()
    local_time = time.localtime(ct)
    data_head = time.strftime("%Y%m%d%H%M%S", local_time)
    data_secs = (ct - int(ct)) * 1000
    time_stamp = "%s_%03d" % (data_head, data_secs)
    return time_stamp

def outputJpg(file):
    with open(file,'rb+') as f1:
        byt = f1.read(4)
        while (byt):
            if b'\x0c\x00\x00\x00' == byt:
                byt = f1.read(4)
                if b'\x01\x00\x00\x00' == byt:
                    # 判断照片缩略图位置
                    list1 = []
                    length = ''
                    byt = f1.read(4)
                    for i in byt:
                        list1.append(i)
                    list1.reverse()
                    for i in list1:
                        length = length + hex(i).replace("0x",'').zfill(2)
                    length = math.ceil(int(length,16) / 4) * 4
                    content = f1.read(length)
                    fileName = get_time_stamp()
                    with open((fileName + '.jpg'),"wb+") as f2:
                        f2.write(content)
                        f2.close()
            byt = f1.read(4)
        f1.close()

def main(argv):
    outputJpg(argv[1])

if __name__ == "__main__":
    main(sys.argv)