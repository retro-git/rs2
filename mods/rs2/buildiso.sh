#!/bin/sh

# clean/compile/build mod iso
python3 ../../../../tools/mod-builder/main.py 11
python3 ../../../../tools/mod-builder/main.py 1 $1
python3 ../../../../tools/mod-builder/main.py 4 $1

# build loader
cd loader/src
make
mv loader.ps-exe ../../../../build/spyro2_rs2/LOADER.BIN # move loader to build folder

# make backup copy of original xml, insert loader into new xml
cd ../../../../build
# rename files from initial build
find . -type f -name 'spyro2_rs2.*' | while read FILE ; do
    newfile="$(echo ${FILE} |sed -e 's/spyro2_rs2/spyro2_rs2_noloader/')" ;
    mv "${FILE}" "${newfile}" ;
done 

# make copy of xml, insert loader, and rename bin/cue
cp spyro2_rs2_noloader.xml spyro2_rs2.xml
sed -i $'/\/directory_tree.*/i\\\\t\\t\\t<file name="LOADER.BIN" source="spyro2_rs2/LOADER.BIN" type="data" />' spyro2_rs2.xml
sed -i 's/mkpsxiso/spyro2_rs2/g' spyro2_rs2.xml

# make backup copy of original system.cnf, insert loader into new system.cnf
cd spyro2
mv SYSTEM.CNF SYSTEM.CNF.ORIG
cp SYSTEM.CNF.ORIG SYSTEM.CNF
sed -i 's/SCUS_944.25/LOADER.BIN/' SYSTEM.CNF

cd ../
mkpsxiso spyro2_rs2.xml -y