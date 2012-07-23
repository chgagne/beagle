#!/bin/bash
ALLFILES=`ls ../src/Beagle/EC.hpp ../src/Beagle/EC/*.?pp`
for F in $ALLFILES; do
  astyle --style="k&r" --indent=tab=4 --indent-col1-comments --lineend=linux $F
  rm $F.orig
done
