import os 
import os.path 
import shutil 
import time,  datetime

# copy Ginkgo_Driver.dll to /bin/Debug
def copyFile(srcPath,rootDir):  
    if os.path.exists(srcPath):  
        shutil.copy(srcPath,listDir(rootDir))  
# list all directory
def listDir(rootDir):
	filelist=os.listdir(rootDir) 
	for f in filelist:
		file = os.path.join( rootDir, f ) 
		if os.path.isdir(file) and '\\bin\\Debug' in str(file):
			return file

# remove all file exception excList
def removeFiles(rootDir,excList):
	filelist=os.listdir(rootDir) 
	for f in filelist:
		file = os.path.join( rootDir, f ) 
		if os.path.isfile(file) and not f in excList: 
			os.remove(file)  
			print (file+" removed!") 
		elif os.path.isdir(file):
			removeFiles(file,excList)
# remove directory
def removeDirs(rootDir):
	filelist=os.listdir(rootDir)  
	for f in filelist:  
		file = os.path.join( rootDir, f )
		if os.path.isdir(file):  
			shutil.rmtree(file,True)  
			print ("dir "+file+" removed!") 
	os.rmdir(rootDir)

srcPath = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\vs2010\\Ginkgo_Driver\\Release\\Ginkgo_Driver.dll'
def backupProject(dir):  
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in dirs:
			if name == 'bin':
				removeFiles(os.path.join(root, name),())
				copyFile(srcPath,os.path.join(root, name))
			if name == 'obj':
				removeFiles(os.path.join(root, name),())
				removeDirs(os.path.join(root, name))


if __name__ == '__main__': 
	backupProject(os.getcwd())
	print('Success!')
