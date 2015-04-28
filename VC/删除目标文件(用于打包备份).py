import os 
import os.path 
import shutil 
import time,  datetime

# copy Ginkgo_Driver.dll to project directory
def copyFile(srcPath,rootDir):  
	if os.path.exists(srcPath):
		obj = listDir(rootDir)
		if None != obj:
			shutil.copy(srcPath,obj)  
# list all directory
def listDir(rootDir):
	filelist=os.listdir(rootDir) 
	for f in filelist:
		file = os.path.join( rootDir, f ) 
		if os.path.isdir(file):
			dllDirList = os.listdir(file)
			if 'Ginkgo_Driver.dll' in dllDirList:
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

dllsrcPath = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\vs2010\\Ginkgo_Driver\\Release\\Ginkgo_Driver.dll'
libsrcPath = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\vs2010\\Ginkgo_Driver\\Release\\Ginkgo_Driver.lib'

def copyLibFiles():
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in dirs:# Copy Ginkgo_Driver.dll and Ginkgo_Driver.lib
			copyFile(dllsrcPath,os.path.join(root, name))
			copyFile(libsrcPath,os.path.join(root, name))

def backupProject():  
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in files:# Remove .sdf file
			if '.sdf' in name:
				os.remove(os.path.join(root, name))
			if '.cmd' in name:
				os.remove(os.path.join(root, name))
		for name in dirs:# Remove buil directory
			if name == 'Debug' or name == 'ipch':
				removeFiles(os.path.join(root, name),())
				removeDirs(os.path.join(root, name))


if __name__ == '__main__': 
    backupProject()
    print('Clear Success!')
    copyLibFiles()
    print('Copy File Success!')
