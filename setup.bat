mkdir lib/bin
cd lib/netcpp
cmake . && cmake --build . --target netcpp --config Release
cd ../oneTBB
cmake . && cmake --build . --target tbb --config Release

for /r .\lib\oneTBB\ %%i in (*.lib, *.dll) do (
    copy "%%i" "lib\bin"
)

for /r .\lib\netcpp\ %%i in (*.lib) do (
    copy "%%i" "lib\bin"
)