#! /bin/bash
time2=$(date "+%Y-%m-%d-%H-%M-%S")
echo $time2
echo $1
workpath=$1
sub="-20"
cd $workpath
dir=$(ls -l |awk '!/^d/ {print $NF}')
for i in $dir
do
    echo "File:"$i
    filename=$i
    if [[ $filename =~ $sub ]]
    	then
    		p='expr index $filename "-20"'
    fi
    newfn=${filename}"-"${time2}
    echo "newfilename : "$newfn
    mv ${filename} ${newfn}
done

#dir=$(ls -l |awk '/^d/ {print $NF}')
#for i in $dir
#do
#    echo "Directory:"$i
#done
# THIS IS A TEST FOR GIT.
