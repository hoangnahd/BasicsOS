#!/bin/bash

#  Kiem tra xem folder PNG hay JPG da tao chua neu chua thi tao
if [ ! -d "PNG" ]; then
    mkdir PNG
fi

if [ ! -d "JPG" ]; then
    mkdir JPG
fi

# Chuyen tat ca file png vao thu muc PNG
mv *.png PNG/ 2>/dev/null

# Dem tung thu muc png va in ra man hinh
png_count=$(find PNG -type f -name "*.png" | wc -l)
echo "So luong file png: $png_count"

# Chuyen tat ca file jpg vao thu muc JPG
mv *.jpg JPG/ 2>/dev/null

# Dem tat ca thu muc jpg va in ra man hinh
jpg_count=$(find JPG -type f -name "*.jpg" | wc -l)
echo "So luong file jpg: $jpg_count"
