#!/bin/sh
set -e -x

export EPICS_HOST_ARCH=`sh $HOME/.source/epics-base/startup/EpicsHostArch`
export EPICS_PVA_ADDR_LIST=127.255.255.255

make -j2 $EXTRA

if [ "$TEST" != "NO" ]
then
  make tapfiles
  make -j2 -s test-results
fi
