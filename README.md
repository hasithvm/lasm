p86asm -- replacement assembler for the p86 virtual machine


Incomplete alpha build of revised assembler.

Missing features:
Write-to-memory operations are not supported, but I/O is.
Register-displacement addressing mode is not supported.
Selectable file output, currently hardcoded to dump "out.obj".


Currently compiles only with GCC and C++11 support. 


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


