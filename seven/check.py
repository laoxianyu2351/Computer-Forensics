	# -*- coding: gbk -*-
import os
import sys

def RecursiveFileSearch(path):
	if (not os.path.exists(path)):	# 判断文件(文件夹)是否存在
		print(path + "不存在！！")
		return 0
	if (os.path.isfile(path)):
		answer = check(path)
		if (answer == 1):
			print(path + "只是个JPG文件")
		elif (answer == 0):
			print(path + "是个JPG文件，且里面存在隐藏的数据！")
	elif (os.path.isdir(path)):
		dirlist = os.listdir(path)
		for dir in dirlist:
			dir = path + '\\' + dir
			RecursiveFileSearch(dir)

def check(file):
	# 首先需要检查是不是文件
	fsize = os.path.getsize(file)
	if (fsize <= 4):
		return 2
		# 优先排除掉过小的文件，避免过多的文件读取
	with open(file,'rb+') as f1:
		temp = f1.read(2)
		f1.close()
	with open(file,'rb') as f2:
		f2.seek(-2,2)
		temp2 = f2.read(2)
		f2.close()

	if (temp == b'\xff\xd8'):
		# 这个ffd8是jpg文件开头
		if (temp2 == b'\xff\xd9'):
			# 这个ffd9是jpg文件结尾
			return 1
			# 文件是jpg文件，尾部没有其他的东西嗷
		else:
			return 0
			# 貌似不是正常的JPG，尾部有东西！
	else:
		return 2
		# 文件头有问题，至少从头查看不是JPG

def main(argv):
	RecursiveFileSearch(argv[1])

if __name__ == "__main__":
	# 下面提供两种启动的方式，一种使用了sys库，使用的是命令行，另外一种是简单地input函数
	# main(sys.argv)
	path = input("请输入路径：")
	RecursiveFileSearch(path)
