#!/bin/bash
rm -rf runTrack logerr mac_htc subjobs output
mkdir runTrack logerr mac_htc subjobs output
#------------  create htcjon_Job_Acc*.sh---------- 
ID_start=100
ID_end=599
NUMinONESH=1
echo "create htcjon_Job.sh"

PACKAGEDIR=`pwd`
CURRENTPATH=`pwd`
TRACKPATH="${CURRENTPATH}/runTrack"
MACHTCPATH="${CURRENTPATH}/mac_htc"
LOGERRPATH="${CURRENTPATH}/logerr"
OUTPUTPATH="${CURRENTPATH}/output"
npar_tous=100
npar_coul=20
npar_brem=20

#------------  create htcjon_Job_Acc*.sh----------
let numsh=0
let j=${NUMinONESH}
for i in `seq ${ID_start} ${ID_end}`
do
  if [[ ${j} -eq ${NUMinONESH} ]]; then
    let j=0
    let numsh=$numsh+1
    cat > ${MACHTCPATH}/htc_Job_${numsh}.sh<<EOF
#!/bin/bash
alias sad='/home/shihc/ustcfs/STCFworkspace/SAD/SAD_Ver.1.1.8/bin/gs'
cd ${MACHTCPATH}
starttime=\$(date +%s)
EOF
  chmod 755 ${MACHTCPATH}/htc_Job_${numsh}.sh
  fi
  cat >> ${MACHTCPATH}/htc_Job_${numsh}.sh<<EOF
  rm -rf ${LOGERRPATH}/Job_${i}.log ${LOGERRPATH}/Job_${i}.err
  ( ( time sad ${TRACKPATH}/Tracking.sad ${TRACKPATH}/TrackingInitT_${numsh}.sad ) >> ${LOGERRPATH}/Job_${i}.log )>& ${LOGERRPATH}/Job_${i}.err
  ( ( time sad ${TRACKPATH}/Tracking.sad ${TRACKPATH}/TrackingInitC_${numsh}.sad ) >> ${LOGERRPATH}/Job_${i}.log )>& ${LOGERRPATH}/Job_${i}.err
  ( ( time sad ${TRACKPATH}/Tracking.sad  ${TRACKPATH}/TrackingInitB_${numsh}.sad ) >> ${LOGERRPATH}/Job_${i}.log )>& ${LOGERRPATH}/Job_${i}.err
 endtime=\$(date +%s)
 time=\$(( (\$endtime - \$starttime) ))
 echo time: \$time s >> ${LOGERRPATH}/Job_${i}.log
EOF
  let j=${j}+1
done

#------------  create Tracking_*.sad and TrackingInit_*.sad----------
cp Tracking.sad ${TRACKPATH}
for i in `seq 1 ${numsh}`
do
  #-------- set min and max ------
  minT=`awk 'BEGIN{printf "%.4f\n",-0.2+(0.2+0.2)*('${i}'-1)/'${numsh}'}'`
  maxT=`awk 'BEGIN{printf "%.4f\n",-0.2+(0.2+0.2)*'${i}'/'${numsh}'}'`
  minC=`awk 'BEGIN{printf "%.4f\n",0+(0.1-0)*('${i}'-1)/'${numsh}'}'`
  maxC=`awk 'BEGIN{printf "%.4f\n",0+(0.1-0)*'${i}'/'${numsh}'}'`
  minB=`awk 'BEGIN{printf "%.4f\n",0+(0.1-0)*('${i}'-1)/'${numsh}'}'`
  maxB=`awk 'BEGIN{printf "%.4f\n",0+(0.1-0)*'${i}'/'${numsh}'}'`
  cat > ${TRACKPATH}/TrackingInitT_${i}.sad<<EOF
REPLACEWORKDIR="${PACKAGEDIR}";
REPLACEMODE="T";
REPLACEDXMIN=${minT};
REPLACEDXMAX=${maxT};
REPLACENPAR=${npar_tous};
REPLACERDSEED=${i};
REPLACEOUTPUT="${OUTPUTPATH}/TousTracking_${i}.txt";
EOF
  cat > ${TRACKPATH}/TrackingInitC_${i}.sad<<EOF
  REPLACEWORKDIR="${PACKAGEDIR}";
  REPLACEMODE="C";
  REPLACEDXMIN=${minC};
  REPLACEDXMAX=${maxC};
  REPLACENPAR=${npar_coul};
  REPLACERDSEED=${i};
  REPLACEOUTPUT="${OUTPUTPATH}/CoulTracking_${i}.txt";
EOF
  cat > ${TRACKPATH}/TrackingInitB_${i}.sad<<EOF
  REPLACEWORKDIR="${PACKAGEDIR}";
  REPLACEMODE="B";
  REPLACEDXMIN=${minB};
  REPLACEDXMAX=${maxB};
  REPLACENPAR=${npar_brem};
  REPLACERDSEED=${i};
  REPLACEOUTPUT="${OUTPUTPATH}/BremTracking_${i}.txt";
EOF
done

#-------------create runbash*.sh--------
for i in `seq 1 ${numsh}`
do
  cat > ${MACHTCPATH}/runbash_${i}.sh<<EOF
  sh ${MACHTCPATH}/htc_Job_${i}.sh
EOF
chmod 755 ${MACHTCPATH}/runbash_${i}.sh
done
#------------- create .cmd job -----------
subdir="${CURRENTPATH}/subjobs"

for i in `seq 1 ${numsh}`
do
  echo "create submit_${i}"
  cat > ${subdir}/submit_${i}.cmd<<EOF
Universe             = vanilla
Notification         = Never
GetEnv               = True
Executable           = ${MACHTCPATH}/runbash_${i}.sh
Output               = $subdir/${i}.out
Error                = $subdir/${i}.err
Log                  = $subdir/${i}.log
+Group               = "BESIII"
#should_transfer_files= yes
#transfer_input_files =
requirements         = (substr(Machine,0,4)!="bl-0"&&ARCH=="X86_64")
WhenToTransferOutput = ON_EXIT_OR_EVICT
OnExitRemove         = TRUE
Queue
EOF
done

#-------- change num in sub.sh----
sed -i "/^ID_end=*/c ID_end=${numsh}" Jobsub.sh

#-------- change num in AnaInRoot/GeneIRRoot.C----
sed -i "s/NUMFILE/${numsh}/g" AnaInRoot/GeneIRRoot.C
