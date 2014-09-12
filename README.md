#lasm -- replacement assembler for the p8086 virtual machine



##Features
* Generates p8086-compatible assembly file.
* Mostly backward-compatible syntax with virgo-assembler.
* Generates listing file for each source file with start addresses and encodings
* Fast flex/bison powered parser with support for multiple immediate formats.

*Builds across multiple platforms with QMake* 

## Usage
```bash
    lasm [-o outputfile] <input-file>
```
## Building
The following instructions summarize the build process for lasm and lasm-gui (optional)

### Prerequisites

#### Windows

Qt 4.8+ should be installed, and QTDIR should point to a QT4.x directory to run qmake.
Install flex and bison either throgh Git for Windows or Cygwin, and the binaries should be runnable through PATH.

PATH should also include a Python 2/3 environment for running some of the build scripts. 

#### Linux

lasm depends on flex, bison, and lasm-gui adds dependencies on qt4. Installing qt4-defaults should pull in the required dependencies.

### Running QMake

QMake generates the necessary VCxx/Xcode/Makefile projects.

```bash
qmake lasm.pro
```

#### Linux/Debian

It's recommended that you run ```make``` followed by ```checkinstall``` to generate a .deb file that's easily installable.


## License
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


