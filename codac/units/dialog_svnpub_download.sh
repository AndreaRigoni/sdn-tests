#!/bin/bash

URL=${1-https://svnpub.iter.org/codac/iter}
DIR=${2-codac/dev/units}
BRA=${3-trunk}
USR=${SVN_USER}

i=0
_fun() {
 for f in $(svn --username ${USR} ls $URL/$DIR); do
  [ -d $f ] && _st="on" || _st="off";
  echo "$f $i $_st"; ((i++));
 done
}

# _fun
list=$(_fun)
dialog --backtitle "Select units" --checklist "Select option:" 50 80 50 $list 2> ./.list.tmp

for f in $(cat ./.list.tmp); do
 echo "retriving $f"
 echo svn --username ${USR} co $URL/$DIR/$f/$BRA $f
 svn --username ${USR} co $URL/$DIR/$f/$BRA $f
 if [ $? -ne 0 ]; then
  echo "repo $f not found. Looking in trunk..."
  svn --username ${USR} co $URL/$DIR/$f/trunk $f
 fi
done


if [ -f ".list.tmp" ]; then
 rm .list.tmp
fi
