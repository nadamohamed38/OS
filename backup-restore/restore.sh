dir=$1
backupdir=$2

choice=0
n=2


while [ $choice -ne 3 ]
do
	read -p "enter 1 to restore back , 2 to restore forward or 3 to exit: " choice


	if [ $choice -eq 1 ]
	then
		n=`expr $n + 1`
		
		file=$(ls -lt "$backupdir" | awk -v var="$n" 'NR==var {print $9}')
		
		if [ -e $file ]
		then
			
			n=`expr $n - 1`
			echo "no older backup to restore "
		else
			rm -r $dir/*
			cp -r $backupdir/$file/* $dir
			echo "restored a previous version $file"
		fi
		
	fi
	
	if [ $choice -eq 2 ]
	then
		if [ $n -eq 2 ]
		then
			echo "no newer backup to restore"
		else
			n=`expr $n - 1`
			file=$(ls -lt "$backupdir" | awk -v var="$n" 'NR==var {print $9}')
			rm -r $dir/*
			cp -r $backupdir/$file/* $dir
			echo "restored a next version $file"
		fi
	fi
	
done 
exit 0
