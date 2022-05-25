#!/bin/bash

updateZip="/home/lhy/project/byjs/middlewareupdate/out/midUpdate/update.zip"
unzipPath="/home/lhy/project/byjs/middlewareupdate/out/midUpdate"
middlewareDir="/home/lhy/project/byjs/middlewareupdate/out/software/update"

rm -rf ${unzipPath}/update
unzip ${updateZip} -d ${unzipPath}    #unzip to destDir, converage the files with  same names
cp -ru ${unzipPath}/update/* ${middlewareDir}
rm -rf ${unzipPath}/update
