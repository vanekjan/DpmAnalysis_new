#!/bin/bash

#copylist - Run16 Au+Au @ 200  GeV: startLine 1, maxLine 583172 (picoList_all_new.list), maxLine 618698 (picoList_all_new_2020.list)

#test

#good HFT runs start at line 166378 (picoList_all_new.list), start at line 177194 (picoList_all_new_2020.list) - (RunId > 17062047)
#choose any prediefined number of lines from picoList_all_new.list
#sed -n '550001,550100 p' ./picoLists/picoList_all_new.list > picoList_test.list
sed -n '500001,550000 p' ./picoLists/picoList_all_new_2020.list > picoList_test.list
#sed -n '300001,301000 p' ./picoLists/picoList_all_new_2020.list > picoList_test.list

#sort ./picoLists/sublists/picoList_test.list > ./picoLists/sublists/picoList_test_09.list

#rm ./picoLists/sublists/picoList_test.list


path=`pwd -P`
path=$( echo $path | sed 's|//|/|g' )


echo executing submitPicoHFMaker.csh f0r picoList_test.list inside $path

starver SL17d
#pre-compile run macro to avoind compilation issues in scratch
root -b -q compileRunMacroLocally.C

#for list generated directly by sed ad the top of this macro
csh starSubmit/submitPicoHFMaker.csh $path picoList_test.list

#for pre-generated sublists
#csh starSubmit/submitPicoHFMaker.csh $path ./picoLists/sublists/picoList_test_08.list
