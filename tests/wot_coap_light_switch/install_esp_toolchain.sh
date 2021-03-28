#!/bin/bash
echo Installing ESP Toolchain...
mkdir -p $HOME/esp
cd $HOME/esp
git clone https://github.com/gschorcht/xtensa-esp32-elf.git
cd $HOME/esp
git clone https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout f198339ec09e90666150672884535802304d23ec
git submodule init
git submodule update
echo Installation complete
