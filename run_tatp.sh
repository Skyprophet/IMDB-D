#!/bin/bash

echo starting TATP tests...

mkdir -p ans_tatp

echo running serial no partition...
./bin/tatp_main 0 1 1 10 > ans_tatp/tatp_serial_1p.txt

echo running serial 100 partitions...
./bin/tatp_main 0 1 100 10 > ans_tatp/tatp_serial_100p.txt

echo running TP batch 1 partition...
for ((t = 1; t <= 5; ++t))
do
	mkdir -p ans_tatp/ans-1p-tp-10ms-${t}
	for ((i = 1; i <= 12; ++i))
	do
		echo running ${t} - ${i}...
		./bin/tatp_main 2 ${i} 1 10 > ans_tatp/ans-1p-tp-10ms-${t}/tatp_${i}.txt
	done
done

echo running TP batch 100 partitions...
for ((t = 1; t <= 5; ++t))
do
	mkdir -p ans_tatp/ans-100p-tp-10ms-${t}
	for ((i = 1; i <= 12; ++i))
	do
		echo running ${t} - ${i}...
		./bin/tatp_main 2 ${i} 100 10 > ans_tatp/ans-100p-tp-10ms-${t}/tatp_${i}.txt
	done
done

echo running batch 100 partitions...
for ((t = 1; t <= 5; ++t))
do
	mkdir -p ans_tatp/ans-100p-10ms-${t}
	for ((i = 1; i <= 12; ++i))
	do
		echo running ${t} - ${i}...
		./bin/tatp_main 1 ${i} 100 10 > ans_tatp/ans-100p-10ms-${t}/tatp_${i}.txt
	done
done

echo runnning batch time test...
for ((t = 1; t <= 5; ++t))
do
	mkdir -p ans_tatp/ans-bat-time-${t}
	for i in 5 10 25 50 75 100 150 200
	do
		echo running ${t} - ${i}ms...
		./bin/tatp_main 2 12 100 ${i} > ans_tatp/ans-bat-time-${t}/tatp_${i}ms.txt
	done
done

echo runnning partition test...
for ((t = 1; t <= 5; ++t))
do
	mkdir -p ans_tatp/ans-partition-${t}
	for i in 1 10 20 30 40 50 75 100
	do
		echo running ${t} - ${i}p...
		./bin/tatp_main 2 12 ${i} 10 > ans_tatp/ans-partition-${t}/tatp_${i}p.txt
	done
done

echo Finished!!
