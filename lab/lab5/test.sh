#Compile the code
# if the file ${cmd} exits then remove it
cmd=lab
if [ -f ${cmd} ]; then
    rm ${cmd}
fi

g++ -std=c++11 *.cpp -o ${cmd}
# if the file ${cmd} does not exit then exit the test
if [ ! -f ${cmd} ]; then
    echo -e "\033[1;91mCompile FAILED.\033[0m"
    exit
fi

# clean existing files 
for casenum in `seq 1 1 3`; do
    if [ -f ${casenum}.out ]; then
        rm ${casenum}.out
    fi
    if [ -f ${casenum}.stderr ]; then
        rm ${casenum}.stderr
    fi
    if [ -f ${casenum}.stdcout ]; then
        rm ${casenum}.stdcout
    fi
    if [ -f output${casenum}.txt ]; then
        rm output${casenum}.txt
    fi
done

# run the script for each test case
for casenum in `seq 1 1 3`; do
	# compile with specific case
    ./${cmd} "input=input${casenum}.txt;output=output${casenum}.txt" 1>${casenum}.stdcout 2>${casenum}.stderr
    
    # compare output.txt with ans.txt, print the difference to diff file
    # -iEBwu means ignore case, tab expansion, blank lines, all spaces
	diff -iEBwu ans${casenum}.txt output${casenum}.txt > ${casenum}.diff
    
    # if diff returns nothing, it means you pass the test case (your output file is correct)
	if [ $? -ne 0 ]; then
        # display "test case 1 FAILED" to the screen. The word "FAILED" is highlighted in Red color
        echo -e "Test case ${casenum}    \033[1;91mFAILED.\033[0m"
	
    else
        # display "test case 1 PASSED" to the screen. The word "PASSED" is highlighted in Green color
        echo -e "Test case ${casenum}    \033[1;92mPASSED.\033[0m"
        # remove the unnecessary files
        rm ${casenum}.diff
        rm ${casenum}.stderr
        rm ${casenum}.stdcout
	fi
done
