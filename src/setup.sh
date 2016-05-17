#!/bin/bash

echo "Installing IBM Messaging..."

INSTALLDIR=$WIND_BASE/pkgs/net/cloud/bluemix/src
cd $INSTALLDIR
if [ -d "$INSTALLDIR/iotf-embeddedc" ];then
    rm iotf-embeddedc/ -rf
fi

# tested with commit 33ab383221ca67e7e3f7a5e297883e7861912fe0 (Fri Mar 11 12:00:57 2016 +0530)
git clone -b master https://github.com/ibm-messaging/iotf-embeddedc.git iotf-embeddedc
sleep 0.5

cd iotf-embeddedc/
git reset --hard 33ab383221ca67e7e3f7a5e297883e7861912fe0
echo "Applying patches to IBM Messaging..."
git apply --whitespace=fix ../patch/iotfclient-vxworks.patch

mv lib mqtt
rm samples -rf
rm buildlib.sh

echo "All done..."
