#!/bin/bash

#copylist - Run16 Au+Au @ 200  GeV (production 2, sst+nosst stream): 
#picoList_all_new_sst_stream.list: startLine 1, maxLine 125,618
#picoList_all_new_sst_stream_2020.list: startLine 1, maxLine 96,689
#choose any prediefined number of lines from picoList_all_new_sst_stream.list
sed -n '66001,96689 p' ./picoLists/picoList_all_new_sst_stream_2020.list > picoList_test_sst.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )


echo executing submitPicoHFMaker.csh f0r picoList_test_sst.list inside $path

csh starSubmit/submitPicoHFMaker.csh $path picoList_test_sst.list
