#!/bin/bash
ALLFILES=`ls ../src/Beagle/Core.hpp ../src/Beagle/EC/*.?pp`
for F in $ALLFILES; do
  ./replheader.py $F $F
done
