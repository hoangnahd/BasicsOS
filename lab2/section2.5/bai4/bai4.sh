#!/bin/sh

echo "Nhap chuoi"

read string

for file in *
do
if grep -l "$string" $file
then
	more $file
fi
done

exit 0
