echo off 
FOR %%n IN (1,2,4,8,10,12,16) DO (
	echo processes: %%n >> out_test_1.txt
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		mpiexec -n %%n "../../DistributedProcessor/Debug/DistributedProcessor.exe" 50000000 %%t >> out_test_1.txt
		timeout 1
	)
)

FOR %%n IN (1,2,4,8,10,12,16) DO (
	echo processes: %%n >> out_test_2.txt
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		mpiexec -n %%n "../../DistributedProcessor/Debug/DistributedProcessor.exe" 50000000 %%t >> out_test_2.txt
		timeout 1
	)
)

FOR %%n IN (1,2,4,8,10,12,16) DO (
	echo processes: %%n >> out_test_3.txt
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		mpiexec -n %%n "../../DistributedProcessor/Debug/DistributedProcessor.exe" 50000000 %%t >> out_test_3.txt
		timeout 1
	)
)

FOR %%n IN (1,2,4,8,10,12,16) DO (
	echo processes: %%n >> out_test_4.txt
	FOR %%t IN (1,2,4,8,10,12,16) DO (
		mpiexec -n %%n "../../DistributedProcessor/Debug/DistributedProcessor.exe" 50000000 %%t >> out_test_4.txt
		timeout 1
	)
)

pause