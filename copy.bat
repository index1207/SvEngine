for /r .\lib\oneTBB\ %%i in (*.lib, *.dll) do (
    copy "%%i" "lib\bin"
)

for /r .\lib\netcpp\ %%i in (*.lib) do (
    copy "%%i" "lib\bin"
)