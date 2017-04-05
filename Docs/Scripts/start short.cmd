echo off 
FOR %%n IN (1,2,4,8,10,12,16) DO (
	echo processes: %%n >> out.txt
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		mpiexec -n %%n DistributedProcessor 50000000 %%t >> out.txt
		timeout 1
	)
)

pause