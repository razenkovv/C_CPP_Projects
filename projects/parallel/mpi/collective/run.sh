#!/bin/bash
if [ $# -lt 1 ]; then echo "please set n_processes"; exit 1; fi
mpiexec -n $1 --oversubscribe ${PWD}/build/run $1