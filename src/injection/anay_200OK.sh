#!/bin/sh
FILE_PATH='/root/work/injection'
date &>> ${FILE_PATH}/log/anay_200OK.log
python ${FILE_PATH}/split_200OK.py &>> ${FILE_PATH}/log/anay_200OK.log
python ${FILE_PATH}/anay_200OK.py &>> ${FILE_PATH}/log/anay_200OK.log
python ${FILE_PATH}/anay_302.py &>> ${FILE_PATH}/log/anay_302.log
date &>> ${FILE_PATH}/log/filter_keyword.log
python ${FILE_PATH}/filter_keyword.py &>> ${FILE_PATH}/log/filter_keyword.log
