#!/bin/bash

URL=${1-https://svnpub.iter.org/codac/iter}
DIR=${2-codac/dev/units}
BRA=${3-trunk}

i=0
_fun() {
 for f in $(svn ls $URL/$DIR); do
  [ -d $f ] && _st="on" || _st="off";
  echo "$f $i $_st"; ((i++));
 done
}

# _fun
list=$(_fun)
dialog --backtitle "Select units" --checklist "Select option:" 50 80 50 $list 2> ./.list.tmp

for f in $(cat ./.list.tmp); do
 echo "retriving $f"
 svn co $URL/$DIR/$f/$BRA $f
done


if [ -f ".list.tmp" ]; then
 rm .list.tmp
fi
