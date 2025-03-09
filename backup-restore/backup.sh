if [ $# -ne 4 ]
then
	echo "$0 must has 4 arguments"
	echo "$0 source destination interval-sec max-backup"
fi

src=$1
dest=$2
intervalsec=$3
maxbackups=$4

if [ $maxbackups -eq 0 ]
then
	echo "you have reached to the maximum backups number"
	exit
fi

if [ ! -e "$dest" ]
then
	mkdir $dest
fi

echo "backup directory is ready: ./$dest"
	create(){
		currentdate=$(date +"%Y-%m-%d-%H-%M-%S")
                mkdir $dest/$currentdate
                cp -r $src/* $dest/$currentdate
                echo "backup created at: ./$dest/$currentdate"
	}

create
n=1

for ((i=0 ; i<=$maxbackups ; ))
do
sleep $intervalsec
	srcdate=$(stat "$src" | grep Modify | awk '{print $3}')
	destdate=$(stat "$dest" | grep Modify | awk '{print $3}')
	
      
       if [[ "$srcdate" > "$destdate" ]];
	then
		create
		i=`expr $i + 1`
	fi
	if [[ $i -eq $maxbackups  ]]
	 then
		i=`expr $i - 1`
		oldest=$(ls -1t "$dest" | tail -n "$n")
		for file in $oldest
		do
			rm -rf "$dest/$file"
		done
		
	 fi
	
done

