	# -*- coding: gbk -*-
import binascii
import struct


def BytToStr(string):
    """此函数的作用就是将读取到的16进制字符流转变成十六进制字符串"""
    hexs = []
    for s in string:
        temp = hex(s).replace('0x','')
        if len(temp) != 2:
            temp.zfill(2)
        # print(temp)
        hexs.append(temp)
    return hexs

if __name__ == "__main__":
    f1 = open(r'1.txt','rb')
    f2 = open(r'2.txt','rb')
    f3 = open(r'3.txt','rb')
    # 打开文件

    hex1 = []
    hex2 = []
    hex3 = []
    # 每一个文件的数据会转换为十六进制被分别存放进其中

    while True:
        temp1 = f1.read()
        temp2 = f2.read()
        temp3 = f3.read()
        # 每次操作先分别读取三个文件512个字节

        if ((temp1 == b'') or (temp2 == b'') or (temp3 == b'')):
            break
        # 判断当读取文件获得为空的时候就直接跳出读取操作
        
        hex1.extend(BytToStr(temp1))
        hex2.extend(BytToStr(temp2))
        hex3.extend(BytToStr(temp3))
        # 将获取到的字符流转换成了十六进制(字符串形式，这个后续还会改成int型)

    f1.close()
    f2.close()
    f3.close()
    # 读取完成关闭文件

    len1 = len(hex1)
    len2 = len(hex2)
    len3 = len(hex3)
    minlen = min(len1,len2,len3)
    # 计算三个文件数据可以无脑异或的部分，后面会解释为啥需要这么做
    midlen = max(min(len1,len2),min(max(len1,len2),len3))
    # 计算中间部分，这个部分属实麻烦……
    maxlen = max(len1,len2,len3)
    # 计算最简单的部分，这个部分都不需要进行异或了，和0异或就是自己本身~

    if (len1 > len2):
        if (len1 > len3):
            if (len2 > len3):
                pass
                # 最好的情况，原本文件长度为1>2>3
            elif (len3 > len2):
                hex2,hex3 = hex3,hex2
                # 此时原本的文件长度为1>3>2
        elif (len3 > len1):
            hex1,hex2,hex3 = hex3,hex1,hex2
            # 此时原本的文件长度为3>1>2
    elif (len2 >len1):
        if (len1 > len3):
            hex1,hex2,hex3 = hex2,hex1,hex3
            # 此时原本文件长度为2>1>3
        elif (len3 > len2):
            hex1,hex2,hex3 = hex3,hex2,hex1
            # 此时原本文件长度为3>2>1
        elif (len2 > len3):
            hex1,hex2,hex3 = hex2,hex3,hex1
            # 此时原本文件长度为2>3>1


    fout = open('out.txt','wb+')
    for i in range(minlen):
        a = int(hex1[i][0],16)*16+int(hex1[i][1],16)
        b = int(hex2[i][0],16)*16+int(hex2[i][1],16)
        c = int(hex3[i][0],16)*16+int(hex3[i][1],16)
        d = struct.pack('B',a^b^c)
        fout.write(d)
    # 这是三个文件数据中可以无脑异或的部分，这时候三个文件的数据都是需要进行异或的
    for i in range(minlen,midlen):
        a = int(hex1[i][0],16)*16+int(hex1[i][1],16)
        b = int(hex2[i][0],16)*16+int(hex2[i][1],16)
        c = struct.pack('B',a^b)
        fout.write(c)
    # 这是有一个文件的长度已经结束，但是还有两个文件需要异或的部分
    for i in range(midlen,maxlen):
        a = struct.pack('B',int(hex1[i][0],16)*16+int(hex1[i][1],16))
        fout.write(a)
    # 这是最后只有一个文件还没有结束的时候。

    fout.close()