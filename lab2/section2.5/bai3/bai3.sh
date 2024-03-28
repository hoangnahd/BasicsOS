#!/bin/sh

echo "Nhap vao so n"
read n

sum=0

while [ $n -lt 10 ]
do
	echo "Nhap lai!"
	read n
done

i=1

while [ $i -le $n ]
do
	sum=$((sum + i))
	i=$((i+1))
done

echo "Tong cac so tu 1 den $n la: $sum"

exit 0
