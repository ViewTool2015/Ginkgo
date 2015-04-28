import os 
import os.path 
import shutil 
import time,  datetime
from zipfile import *
import zipfile 

def zip_dir(dirname,zipfilename):
    filelist = []
    if os.path.isfile(dirname):
        filelist.append(dirname)
    else :
        for root, dirs, files in os.walk(dirname):
            for name in files:
                filelist.append(os.path.join(root, name)) 
    zf = zipfile.ZipFile(zipfilename, "w", zipfile.zlib.DEFLATED)
    for tar in filelist:
        arcname = tar[len(dirname)-len(zipfilename)+4:]
        zf.write(tar,arcname)
    zf.close()


def batchPackageAndroidStudio():  
	fileNum = 0
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in dirs:# Remove buil directory
			if 'AndroidStudio_USB' in str(name):
				target = name + '.zip'
				zip_dir(os.path.join(root, name),target)
				print('Package '+target+' Success!')
				fileNum += 1
	return fileNum

def batchPackageEclipse():  
	fileNum = 0
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in dirs:# Remove buil directory
			if 'Eclipse_USB' in str(name):
				target = name + '.zip'
				zip_dir(os.path.join(root, name),target)
				print('Package '+target+' Success!')
				fileNum += 1
	return fileNum

if __name__ == '__main__': 
	fileNum = batchPackageAndroidStudio()
	print('%d Package File Success!'%fileNum)
	fileNum = batchPackageEclipse()
	print('%d Package File Success!'%fileNum)