procID=`pgrep naga`
if [ "" == "$procID" ];
then
	cd /home/naga/bin/
	/home/naga/bin/naga --NODPDK em2 &
fi
