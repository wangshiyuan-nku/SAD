#!/bin/bash

#----------submit jobs------
workdir=`pwd`
subdir="${workdir}/subjobs"
ID_start=1
ID_end=500
for i in `seq ${ID_start} ${ID_end}`
do
  echo "submit No:${count}"
  condor_submit ${subdir}/submit_${i}.cmd
done

