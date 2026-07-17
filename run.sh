#!/bin/bash
set -euo pipefail

if [ ! -d "hermes_v250829098.0.16" ]; then
	echo "Cloning and building hermes_v250829098.0.16"

	git clone https://github.com/facebook/hermes.git hermes_v250829098.0.16
	cd hermes_v250829098.0.16
	git checkout hermes-v250829098.0.16
	cmake . -B cmake_build -DCMAKE_BUILD_TYPE=Release
	cd cmake_build && make -j10
	cd ../../

fi


if [ ! -d "hermes_v250829098.0.13" ]; then
	echo "Cloning and building hermes_v250829098.0.13"
	git clone https://github.com/facebook/hermes.git hermes_v250829098.0.13
	cd hermes_v250829098.0.13
	git checkout hermes-v250829098.0.13
	cmake . -B cmake_build -DCMAKE_BUILD_TYPE=Release
	cd cmake_build && make -j10
	cd ../../
fi


if [ ! -d "hermes_RNv0.79.3_7f9a871eefeb2c3852365ee80f0b6733ec12ac3b" ]; then
	echo "Cloning and building hermes_RNv0.79.3_7f9a871eefeb2c3852365ee80f0b6733ec12ac3b"
	git clone https://github.com/facebook/hermes.git hermes_RNv0.79.3_7f9a871eefeb2c3852365ee80f0b6733ec12ac3b
	cd hermes_RNv0.79.3_7f9a871eefeb2c3852365ee80f0b6733ec12ac3b
	git checkout hermes-2025-06-04-RNv0.79.3-7f9a871eefeb2c3852365ee80f0b6733ec12ac3b
	cmake . -B cmake_build -DCMAKE_BUILD_TYPE=Release
	cd cmake_build && make -j10
	cd ../../
fi

echo "Building memory_sampler"
cd memory_sampler
cmake . -B build -DCMAKE_BUILD_TYPE=Release
cd build && make

cd ../../


echo "Running memory sampler ..."
if [ ! -d "results" ]; then
	mkdir results
fi
memory_sampler/build/memory_sampler hermes $PWD/results/rn_v1_0.16.csv & 
SAMPLER_1_PID=$!
hermes_v250829098.0.16/cmake_build/bin/hermes $PWD/benchmark.js -gc-print-stats -gc-before-stats
kill -INT $SAMPLER_1_PID


memory_sampler/build/memory_sampler hermes $PWD/results/rn_v1_0.13.csv & 
SAMPLER_2_PID=$!
hermes_v250829098.0.13/cmake_build/bin/hermes $PWD/benchmark.js -gc-print-stats -gc-before-stats
kill -INT $SAMPLER_2_PID


memory_sampler/build/memory_sampler hermes $PWD/results/rn_079.csv & 
SAMPLER_3_PID=$!
hermes_RNv0.79.3_7f9a871eefeb2c3852365ee80f0b6733ec12ac3b/cmake_build/bin/hermes $PWD/benchmark.js -gc-print-stats -gc-before-stats
kill -INT $SAMPLER_3_PID
wait
exit


