## Executable location
clr_wrap\wpf_test\bin\x86\Debug\wpf_test.exe

## Building guide
Generation
```shell
cmake --help
```
```shell
cmake -S . -B ./build/x64 -A x64
```
```shell
cmake -S . -B ./build/Win32 -A Win32
```
Building
```shell
cmake --build ./build/x64 --config Release
```