	# -*- coding: gbk -*-
import binascii
import struct


def BytToStr(string):
    """�˺��������þ��ǽ���ȡ����16�����ַ���ת���ʮ�������ַ���"""
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
    # ���ļ�

    hex1 = []
    hex2 = []
    hex3 = []
    # ÿһ���ļ������ݻ�ת��Ϊʮ�����Ʊ��ֱ��Ž�����

    while True:
        temp1 = f1.read()
        temp2 = f2.read()
        temp3 = f3.read()
        # ÿ�β����ȷֱ��ȡ�����ļ�512���ֽ�

        if ((temp1 == b'') or (temp2 == b'') or (temp3 == b'')):
            break
        # �жϵ���ȡ�ļ����Ϊ�յ�ʱ���ֱ��������ȡ����
        
        hex1.extend(BytToStr(temp1))
        hex2.extend(BytToStr(temp2))
        hex3.extend(BytToStr(temp3))
        # ����ȡ�����ַ���ת������ʮ������(�ַ�����ʽ�������������ĳ�int��)

    f1.close()
    f2.close()
    f3.close()
    # ��ȡ��ɹر��ļ�

    len1 = len(hex1)
    len2 = len(hex2)
    len3 = len(hex3)
    minlen = min(len1,len2,len3)
    # ���������ļ����ݿ����������Ĳ��֣���������Ϊɶ��Ҫ��ô��
    midlen = max(min(len1,len2),min(max(len1,len2),len3))
    # �����м䲿�֣����������ʵ�鷳����
    maxlen = max(len1,len2,len3)
    # ������򵥵Ĳ��֣�������ֶ�����Ҫ��������ˣ���0�������Լ�����~

    if (len1 > len2):
        if (len1 > len3):
            if (len2 > len3):
                pass
                # ��õ������ԭ���ļ�����Ϊ1>2>3
            elif (len3 > len2):
                hex2,hex3 = hex3,hex2
                # ��ʱԭ�����ļ�����Ϊ1>3>2
        elif (len3 > len1):
            hex1,hex2,hex3 = hex3,hex1,hex2
            # ��ʱԭ�����ļ�����Ϊ3>1>2
    elif (len2 >len1):
        if (len1 > len3):
            hex1,hex2,hex3 = hex2,hex1,hex3
            # ��ʱԭ���ļ�����Ϊ2>1>3
        elif (len3 > len2):
            hex1,hex2,hex3 = hex3,hex2,hex1
            # ��ʱԭ���ļ�����Ϊ3>2>1
        elif (len2 > len3):
            hex1,hex2,hex3 = hex2,hex3,hex1
            # ��ʱԭ���ļ�����Ϊ2>3>1


    fout = open('out.txt','wb+')
    for i in range(minlen):
        a = int(hex1[i][0],16)*16+int(hex1[i][1],16)
        b = int(hex2[i][0],16)*16+int(hex2[i][1],16)
        c = int(hex3[i][0],16)*16+int(hex3[i][1],16)
        d = struct.pack('B',a^b^c)
        fout.write(d)
    # ���������ļ������п����������Ĳ��֣���ʱ�������ļ������ݶ�����Ҫ��������
    for i in range(minlen,midlen):
        a = int(hex1[i][0],16)*16+int(hex1[i][1],16)
        b = int(hex2[i][0],16)*16+int(hex2[i][1],16)
        c = struct.pack('B',a^b)
        fout.write(c)
    # ������һ���ļ��ĳ����Ѿ����������ǻ��������ļ���Ҫ���Ĳ���
    for i in range(midlen,maxlen):
        a = struct.pack('B',int(hex1[i][0],16)*16+int(hex1[i][1],16))
        fout.write(a)
    # �������ֻ��һ���ļ���û�н�����ʱ��

    fout.close()