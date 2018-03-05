
exp="heep"
numEvents=-1
runNum=1929
runReportMon="./get_${exp}_report.py ${runNum} ${numEvents}"
out_filename="heep_report_${runNum}.report"

openReport="emacs -d display ./UTIL_ED/REPORT_FILES/${out_filename}"
eval ${runReportMon} 
eval ${openReport}
