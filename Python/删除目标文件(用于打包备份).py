import os 
import os.path 
import shutil 
import time,  datetime

# copy Ginkgo_Driver.dll to project directory
def copyFile(srcPath,rootDir,dllType):  
	if os.path.exists(srcPath):
		obj = listDir(rootDir,dllType)
		if None != obj:
			shutil.copy(srcPath,obj)  
# list all directory
def listDir(rootDir,dllType):
	filelist=os.listdir(rootDir) 
	for f in filelist:
		file = os.path.join( rootDir, f ) 
		if dllType=='win32' and os.path.isdir(file) and '\\lib\\windows\\32bit' in str(file):
			return file
		if dllType=='win64' and os.path.isdir(file) and '\\lib\\windows\\64bit' in str(file):
			return file
		if dllType=='linux32' and os.path.isdir(file) and '\\lib\\linux\\32bit' in str(file):
			return file
		if dllType=='linux64' and os.path.isdir(file) and '\\lib\\linux\\64bit' in str(file):
			return file
		if dllType=='macos' and os.path.isdir(file) and '\\lib\\macos' in str(file):
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

dllsrcPathwin32 = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\vs2010\\Ginkgo_Driver\\Release\\Ginkgo_Driver.dll'
dllsrcPathwin64 = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\vs2010\\Ginkgo_Driver\\x64\\Release\\Ginkgo_Driver.dll'
dllsrcPathlinux32 = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\CodeBlocks\\linux_32bit\\Ginkgo_Driver\\bin\\Release\\libGinkgo_Driver.so'
dllsrcPathlinux64 = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\CodeBlocks\\linux_64bit\\Ginkgo_Driver\\bin\\Release\\libGinkgo_Driver.so'
dllsrcPathmacos = 'E:\\Project\\vt_ginkgo\\truck\\Ginkgo Driver\\CodeBlocks\\macos\\Ginkgo_Driver\\bin\\Release\\libGinkgo_Driver.dylib'
def backupProject():  
	for root, dirs, files in os.walk(os.getcwd(), topdown=False):
		for name in dirs:# Remove buil directory
			copyFile(dllsrcPathwin32,os.path.join(root, name),'win32')
			copyFile(dllsrcPathwin64,os.path.join(root, name),'win64')
			copyFile(dllsrcPathlinux32,os.path.join(root, name),'linux32')
			copyFile(dllsrcPathlinux64,os.path.join(root, name),'linux64')
			copyFile(dllsrcPathmacos,os.path.join(root, name),'macos')
			if name == '__pycache__':
				removeFiles(os.path.join(root, name),())
				os.rmdir(os.path.join(root, name))


if __name__ == '__main__': 
	backupProject()
	print('Success!')
