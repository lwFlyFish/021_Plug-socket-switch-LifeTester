:: �رջ���
@echo off

:: ���õ�ǰĿ¼Ϊ������Ŀ¼
cd /d "%~dp0"

:: ɾ��Ŀ¼�����е�ָ�����Ƶ��ļ���
for /f "delims=" %%i in ('dir /b /s /ad ^| findstr "__Previews"') do (rd /s /q "%%i")
for /f "delims=" %%i in ('dir /b /s /ad ^| findstr "Project Logs for "') do (rd /s /q "%%i")

del vssver2.scc /s

del *.crf /s
del *.d /s
del *.o /s

::ɾ��JLINK�����ļ�  https://www.pianshen.com/article/4704761960/
del JLinkLog.txt /s
del JLinkSettings.ini /s

pause
::exit
