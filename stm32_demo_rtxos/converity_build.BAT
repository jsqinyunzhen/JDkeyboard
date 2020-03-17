rem @echo off

rem 设定环境变量及参数
rem set path=%path%;C:\Program Files\TortoiseSVN\bin\svn.exe
set LANG=zh_CN.UTF-8
set coverity_dir=ci_coverity_dir
rem %JOB_NAME%_%BUILD_ID%
rem 创建Coverity的log存放目录
echo %WORKSPACE%
mkdir %coverity_dir%
rem 执行编译
cov-build.exe --dir "%WORKSPACE%\%coverity_dir%" invoke_build.BAT
rem 判断是否成功
if %errorlevel% NEQ 0 (
echo ">>>>>>>>>>>>>>build faild<<<<<<<<<<<<<<<<<<<<"
for /f "delims=" %%i in ('dir "ci_*" /ad/b') do rd "%%i" /s/q
exit /b 1
)
rem 获取SVN信息进行匹配
rem "D:\IDE\cov-analysis-win64-8.7.0\bin\cov-import-scm" --dir %WORKSPACE%\%coverity_dir% --scm svn
rem 打包Coverity搜集的编译过程信息
rem "C:\Program Files (x86)\7-Zip\7z" a %coverity_dir%.zip %coverity_dir%