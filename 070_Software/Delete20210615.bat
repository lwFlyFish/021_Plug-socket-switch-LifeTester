:: 关闭回显
@echo off

:: 设置当前目录为批处理目录
cd /d "%~dp0"

:: 删除目录下所有的指定名称的文件夹
for /f "delims=" %%i in ('dir /b /s /ad ^| findstr "__Previews"') do (rd /s /q "%%i")
for /f "delims=" %%i in ('dir /b /s /ad ^| findstr "Project Logs for "') do (rd /s /q "%%i")

del vssver2.scc /s

del *.crf /s
del *.d /s
del *.o /s

::删除JLINK配置文件  https://www.pianshen.com/article/4704761960/
del JLinkLog.txt /s
del JLinkSettings.ini /s

pause
::exit
