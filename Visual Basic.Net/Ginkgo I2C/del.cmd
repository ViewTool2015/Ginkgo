del *.exe /s
del *.pdb /s
del *.manifest /s
del *.xml /s
del *.cache /s
del *.resources /s
del *.Cache /s
del *.txt /s
for /f "delims=" %%i in ('dir /s /b /ad ') do rd "%%i"
pause