#!/bin/sh

echo "Ten: "
read ten

echo "MSSV: "
read mssv

while [ "$mssv" != "22520041" ]
do
        echo "Khong trung khop!"
        read mssv
done

echo "Ten: $ten"
echo "MSSV: $mssv"

exit 0

