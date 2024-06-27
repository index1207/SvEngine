@echo off

@rem install python module
pip install stringcase

@rem build libraries
mkdir .\lib\bin\Debug
mkdir .\lib\bin\Release

call :build Debug
call :build Release
EXIT /B %ERRORLEVEL%

:build
cd lib\netcpp   
cmake . && cmake --build . --target netcpp --config %~1
cd ..\oneTBB
cmake . && cmake --build . --target tbb --config %~1
cd ..\..\
for /r .\lib\oneTBB\ %%i in (*.lib, *.dll) do (
    copy "%%i" "lib\bin\%~1\"
    del "%%i"
)

for /r .\lib\netcpp\ %%i in (*.lib) do (
    copy "%%i" "lib\bin\%~1\"
    del "%%i"
)
EXIT /B 0