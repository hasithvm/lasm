#lasm -- replacement assembler for the p8086 virtual machine


Alpha build of revised assembler.

##Missing features
* Register-based addressing mode is not supported.


*Currently compiles with GCC, and Visual Studio 2010*

**Usage**
```bash
    lasm [filename] [outputfile]
```

**Notes for building with Visual Studio**
[GnuWin32](http://gnuwin32.sourceforge.net/) is required to compile lasm,
due to it's dependency on Flex and Bison. The location of gnuwin32's bin
directory must be stored in the ```GNUBINDIR``` environment variable.

##License
Copyright (C) 2012 Hasith Vidanamadura, contributing authors Robert Nelson and Darren Stahl.

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


