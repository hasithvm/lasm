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


