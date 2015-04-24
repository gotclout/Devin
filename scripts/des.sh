#!/bin/bash

export BIN="../bin"
export TESTS="../tests"
export IN="$TESTS/in"
export OUT="$TESTS/out/des"
export NAM="tempdes"

if [ -e $BIN/$NAM ]; then
  echo "Removing old binary..."
  rm $BIN/$NAM;
fi

echo "Compiling $BIN/$NAM..."
gcc -g -lrt -lcrypto ../src/$NAM.c -o $BIN/$NAM

if [ -e $BIN/$NAM ]; then
  echo "Executing encoding tests..."
  rm $OUT/*.des
  rm $OUT/*.txt
  { time $BIN/$NAM $IN/8.txt $OUT/8.des;            } 2> $OUT/time_8.txt
  { time $BIN/$NAM $IN/64.txt $OUT/64.des;          } 2> $OUT/time_64.txt
  { time $BIN/$NAM $IN/512.txt $OUT/512.des;        } 2> $OUT/time_512.txt
  { time $BIN/$NAM $IN/4096.txt $OUT/4096.des;      } 2> $OUT/time_4096.txt
  { time $BIN/$NAM $IN/32768.txt $OUT/32768.des;    } 2> $OUT/time_32768.txt
  { time $BIN/$NAM $IN/262144.txt $OUT/262144.des;  } 2> $OUT/time_262144.txt
  { time $BIN/$NAM $IN/2047152.txt $OUT/2047152.des;} 2> $OUT/time_2047152.txt
  echo "$NAM test complete"
  cat $OUT/time_* | grep real > ../doc/des_time.txt
else
  echo "Could not create executable $BIN/$NAM";
fi

echo "Exiting..."
echo
