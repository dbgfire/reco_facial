#!/bin/sh

cd $1
path=${pwd}	
echo $path
i=1
while [ $i -ne 41 ] ; 
do
cd "./s"${i}
pwd

for file in ./*.pgm;
	do 
		convert ${file} -resize 92x92\! ${file};
	done
  i=$(($i + 1))
cd ../
done
