@ECHO OFF

@rem install python module
pip install stringcase

SET LIB_DIR=.\Libraries\
SET BIN_DIR=.\Binaries\

@rem build libraries
IF NOT EXIST "%BIN_DIR%\Debug" ( MKDIR %BIN_DIR%\Debug )
IF NOT EXIST "%BIN_DIR%\Release" ( MKDIR %BIN_DIR%\Release )

CALL :build Debug
CALL :build Release
EXIT /B %ERRORLEVEL%

:build
CD %LIB_DIR%\netcpp   
cmake . && cmake --build . --target netcpp --config %~1
CD ..\oneTBB
cmake . && cmake --build . --target tbb --config %~1
CD ..\..\
for /r %LIB_DIR%\netcpp\ %%i IN (*.lib, *.dll) do (
    copy "%%i" "%BIN_DIR%\%~1\"
    del "%%i"
)
FOR /r %LIB_DIR%\oneTBB\ %%i IN (*.lib, *.dll) do (
    COPY "%%i" "%BIN_DIR%\%~1\"
    DEL "%%i"
)
EXIT /B 0