currentdir=`pwd`
dirlay2=logerr
cd $dirlay2
for i in `seq 100 599`
do
  Suc=`tail -n 3 Job_${i}.log | grep "time:" | wc -l`
  if [[ Suc -eq 0 ]];then
     echo Job_${i} not success
  fi
done
cd -
