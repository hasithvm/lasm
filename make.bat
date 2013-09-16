mkdir bin
del /Q bin\*
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Debug
cd msvc10
msbuild lasm.sln /nologo /verbosity:m /p:Configuration=Debug
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Release
msbuild lasm.sln /nologo /verbosity:m /p:Configuration=Release
