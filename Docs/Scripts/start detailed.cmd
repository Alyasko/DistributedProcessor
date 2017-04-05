echo off 
FOR %%n IN (1,2,4,8,10,12,16) DO (
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		echo processes: %%n threads: %%t >> out.txt
		mpiexec -n %%n DistributedProcessor 500000 %%t >> out.txt
		timeout 1
	)
)

pause