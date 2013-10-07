#lasm -- replacement assembler for the p8086 virtual machine



##Features
* Generates p8086-compatible assembly file.
* Mostly backward-compatible syntax with virgo-assembler.
* Generates listing file for each source file with start addresses and encodings
* Fast flex/bison powered parser with support for multiple immediate formats.

*Builds with both msvc10/msbuild and gcc/make environments* 

**Usage**
```bash
    lasm [filename] [outputfile]
```
**Building**

***Windows***
The msvc10 build environment currently requires an environment variable GNUBINDIR, pointing to a bin folder with flex and bison >= 2.7.1 (preferably GnuWin32), and a variable FLEXINCLUDE pointing to /usr/include to pull in unistd.h for flex. QTDIR should point to a QT4.x directory to run qmake for lasm-gui.
PATH should include a Python 2/3 environment for running some of the build scripts. 

***Linux***
lasm depends on flex, bison, and lasm-gui adds dependencies on qt4. Installing qt4-defaults should pull in the required dependencies.

```bash
	make
```

##License
Copyright (C) 2013 Hasith Vidanamadura, Robert Nelson, and Darren Stahl.

Original p86asm specification (C) Trevor Pearce (2001)


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


