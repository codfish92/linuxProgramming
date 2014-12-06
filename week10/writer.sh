#/bin/bash

# usage$ ./writer.sh thePipe

for (( i=10000000; i>0; i-- )) do
	#sleep $i
	echo >/dev/tty "Writing $i lines"
	head -$i /usr/share/dict/words
	echo >/dev/tty "Writing $i lines complete"
done > "${1}"
