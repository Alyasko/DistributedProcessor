@echo off

SET itn=20000000

echo Testing started
echo Array size is %itn%

FOR /L %%a IN (1,1,300) DO (
	echo Iteration %%a
	echo iteration %%a >> out/night_pc.txt
	FOR %%n IN (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) DO (
		FOR %%t IN (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) DO (
			mpiexec -n %%n "../../DistributedProcessor/Debug/DistributedProcessor.exe" %itn% %%t >> out/night_pc.txt
			timeout 1
		)
	)
)

pause