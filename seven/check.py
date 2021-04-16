	# -*- coding: gbk -*-
import os
import sys

def RecursiveFileSearch(path):
	if (not os.path.exists(path)):	# �ж��ļ�(�ļ���)�Ƿ����
		print(path + "�����ڣ���")
		return 0
	if (os.path.isfile(path)):
		answer = check(path)
		if (answer == 1):
			print(path + "ֻ�Ǹ�JPG�ļ�")
		elif (answer == 0):
			print(path + "�Ǹ�JPG�ļ���������������ص����ݣ�")
	elif (os.path.isdir(path)):
		dirlist = os.listdir(path)
		for dir in dirlist:
			dir = path + '\\' + dir
			RecursiveFileSearch(dir)

def check(file):
	# ������Ҫ����ǲ����ļ�
	fsize = os.path.getsize(file)
	if (fsize <= 4):
		return 2
		# �����ų�����С���ļ������������ļ���ȡ
	with open(file,'rb+') as f1:
		temp = f1.read(2)
		f1.close()
	with open(file,'rb') as f2:
		f2.seek(-2,2)
		temp2 = f2.read(2)
		f2.close()

	if (temp == b'\xff\xd8'):
		# ���ffd8��jpg�ļ���ͷ
		if (temp2 == b'\xff\xd9'):
			# ���ffd9��jpg�ļ���β
			return 1
			# �ļ���jpg�ļ���β��û�������Ķ����
		else:
			return 0
			# ò�Ʋ���������JPG��β���ж�����
	else:
		return 2
		# �ļ�ͷ�����⣬���ٴ�ͷ�鿴����JPG

def main(argv):
	RecursiveFileSearch(argv[1])

if __name__ == "__main__":
	# �����ṩ���������ķ�ʽ��һ��ʹ����sys�⣬ʹ�õ��������У�����һ���Ǽ򵥵�input����
	# main(sys.argv)
	path = input("������·����")
	RecursiveFileSearch(path)
