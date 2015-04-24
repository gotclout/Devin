#!/bin/bash

export BIN="../bin"
export TESTS="../tests"
export IN="$TESTS/in"
export OUT="$TESTS/out/sha1"
export NAM="tempsha1"

if [ -e $BIN/$NAM ]; then
  echo "Removing old binary..."
  rm $BIN/$NAM;
fi

echo "Compiling $BIN/$NAM..."
gcc -g -lcrypto ../src/$NAM.c -o $BIN/$NAM

if [ -e $BIN/$NAM ]; then
  echo "Executing SHA1 digest tests..."
  rm $OUT/*.sha1
  rm $OUT/*.txt
  { time $BIN/$NAM $IN/2.txt $OUT/2.sha1;    } 2> $OUT/time_2.txt
  { time $BIN/$NAM $IN/4.txt $OUT/4.sha1;    } 2> $OUT/time_4.txt
  { time $BIN/$NAM $IN/8.txt $OUT/8.sha1;    } 2> $OUT/time_8.txt
  { time $BIN/$NAM $IN/16.txt $OUT/16.sha1;  } 2> $OUT/time_16.txt
  { time $BIN/$NAM $IN/32.txt $OUT/32.sha1;  } 2> $OUT/time_32.txt
  { time $BIN/$NAM $IN/64.txt $OUT/64.sha1;  } 2> $OUT/time_64.txt
  { time $BIN/$NAM $IN/128.txt $OUT/128.sha1;} 2> $OUT/time_128.txt
  echo "$NAM test complete"
  cat $OUT/time_* | grep real > ../doc/sha_time.txt
else
  echo "Could not create executable $BIN/$NAM";
fi

echo "Exiting..."
echo
