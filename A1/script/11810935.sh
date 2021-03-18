#!/bin/bash
original_IFS=${IFS}
IFS=$'\n\t\n'
base_name=$(basename $2)
dir_name=$(dirname $2)
cd ${dir_name}
output="$(pwd)/${base_name}"
cd $1
target=$(pwd)
count=0
point=0
files=0
arr[0]=${target}
count=$(($count+1))
while [ ${point} -lt ${count} ]
do
	path=${arr[${point}]}
	point=$(($point+1))
	cd ${path}
	base_name=$(basename `pwd`)
	echo "[${base_name}]" >> ${output}
	for i in `ls`
	do
		abs_path="$(pwd)/$i"
		echo $abs_path >> ${output}
		if [ -d $i ]
		then
			arr[${count}]=${abs_path}
			count=$(($count+1))
		else
			files=$(($files+1))
		fi
	done
	echo  >> ${output}
done
count=$(($count-1))
echo "[Directories Count]:${count}" >> ${output}
echo "[Files Count]:${files}" >> ${output}
IFS=${original_IFS}
