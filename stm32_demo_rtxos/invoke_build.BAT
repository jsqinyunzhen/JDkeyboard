@echo off
echo "Current dir:%~dp0"
echo "C2_Extra start build, %DATE:~0,4%/%DATE:~5,2%/%DATE:~8,2% %TIME:~0,2%:%TIME:~3,2%:%TIME:~6,2%"
rem 创建编译日志目录
set log_dir=log
if not exist %log_dir% (
echo "%log_dir% folder doset not exist, create it"
mkdir %log_dir%
)
rem Build stm32_rtxos
echo "Build stm32_rtxos Start"

if exist "stm32_rtxos.uvprojx" (
echo "Find stm32_rtxos.uvprojx"
C:\Keil_v5\UV4\UV4 -r stm32_rtxos.uvprojx -o "%~dp0\%log_dir%\stm32_rtxos.uvprojx.txt"
echo "stm32_rtxos.uvprojx finished"
cd %~dp0\%log_dir%
for /f "tokens=2*" %%i in (stm32_rtxos.uvprojx.txt) do (
if "%%i"=="warning:" (
echo %%i %%j
)
if "%%i"=="error:" (
echo %%i %%j
)
)
echo "stm32_rtxos.uvprojx Build End, %DATE:~0,4%/%DATE:~5,2%/%DATE:~8,2% %TIME:~0,2%:%TIME:~3,2%:%TIME:~6,2%"
for /f "tokens=3,4" %%i in (stm32_rtxos.uvprojx.txt) do (
if "%%i"=="0" (
if "%%j"=="Error(s)," (
echo "Build stm32_rtxos Ok"
echo "Build SUCCESS!"
exit /b 0
)
)
)
)
echo "Build stm32_rtxos Fail"
echo "Build Failed!"
exit /b 1
