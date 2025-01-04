#!/bin/sh

LD_LIBRARY_PATH="/lrya/lib"
export LD_LIBRARY_PATH

export ANDROID_DATA=/data ANDROID_ROOT=/lrya

echo "Running crash tests..."

./mate-testcrash SIGABRT 0.25 2
./mate-testcrash SIGFPE 0.25 2
./mate-testcrash SIGILL 0.25 2
./mate-testcrash SIGSEGV 0.25 2

./mate-testcrash null 0.5 10

./mate-testcrash abort 1 10

./mate-testcrash stackoverflow 0.25 2 

# Normal exit:  Times out before crashing
./mate-testcrash null 999 0.5

echo "Done running crash tests."
