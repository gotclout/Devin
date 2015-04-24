$BIN/bash

export BIN="../bin"
export TESTS="../tests"
export IN="$TESTS/in"
export OUT="$TESTS/out/rsa"
export NAM="temprsa"

if [ -e $BIN/$NAM ]; then
  echo "Removing old binary..."
  rm $BIN/$NAM;
fi

echo "Compiling $BIN/$NAM..."
gcc -g -lrt -lcrypto ../src/$NAM.c -o $BIN/$NAM

if [ -e $BIN/$NAM ]; then
  echo "Executing encoding tests..."
  rm $OUT/*.rsa
  rm $OUT/*.txt
  { time $BIN/$NAM $IN/8.txt $OUT/8.rsa 1;            } 2> $OUT/time_8.txt
  { time $BIN/$NAM $IN/64.txt $OUT/64.rsa 1;          } 2> $OUT/time_64.txt
  { time $BIN/$NAM $IN/512.txt $OUT/512.rsa 1;        } 2> $OUT/time_512.txt
  { time $BIN/$NAM $IN/4096.txt $OUT/4096.rsa 1;      } 2> $OUT/time_4096.txt
  { time $BIN/$NAM $IN/32768.txt $OUT/32768.rsa 1;    } 2> $OUT/time_32768.txt
  { time $BIN/$NAM $IN/262144.txt $OUT/262144.rsa 1;  } 2> $OUT/time_262144.txt
  { time $BIN/$NAM $IN/2047152.txt $OUT/2047152.rsa 1;} 2> $OUT/time_2047152.txt

  echo "Press D decode output files, or any key to continue"
  read dopt
  echo

  if [ "$dopt" == "D" ]; then
    echo "Executing decoding tests..."
    { time $BIN/$NAM $OUT/8.rsa $OUT/8D.txt 0;            } 2> $OUT/timeD_8.txt
    { time $BIN/$NAM $OUT/64.rsa $OUT/64D.txt 0;          } 2> $OUT/timeD_64.txt
    { time $BIN/$NAM $OUT/512.rsa $OUT/512D.txt 0;        } 2> $OUT/timeD_512.txt
    { time $BIN/$NAM $OUT/4096.rsa $OUT/4096D.txt 0;      } 2> $OUT/timeD_4096.txt
    { time $BIN/$NAM $OUT/32768.rsa $OUT/32768D.txt 0;    } 2> $OUT/timeD_32768.txt
    { time $BIN/$NAM $OUT/262144.rsa $OUT/262144D.txt 0;  } 2> $OUT/timeD_262144.txt
    { time $BIN/$NAM $OUT/2047152.rsa $OUT/2047152D.txt 0;} 2> $OUT/timeD_2047152.txt
  fi
  echo "$NAM test complete"
else
  echo "Could not create executable $BIN/$NAM";
fi

echo "Exiting..."
echo
