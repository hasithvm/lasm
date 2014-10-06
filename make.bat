mkdir bin
mkdir msvc10
del /Q bin\*
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Debug
cd msvc10
%QTDIR%\bin\qmake -spec win32-msvc2010 -tp vc -r ..
msbuild lasm.sln /nologo /verbosity:m /p:Configuration=Debug
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Release
msbuild lasm.sln /nologo /verbosity:m /p:Configuration=Release
copy lasm-gui/bin/* ../bin/
copy src/bin/* ../bin/