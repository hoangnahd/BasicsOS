#!/bin/bash

# Yeu cau nguoi dung nhap ten
echo "Vui long nhap ho va ten: "
read name

# Create a folder with the entered name
mkdir "$name"

# Kiem tra xem thu muc da duoc tao thanh cong chua
if [ $? -eq 0 ]; then
    echo "Thu muc '$name' da duoc tao thanh cong"
else
    echo "Thu muc da duoc tao khong thanh cong '$name'."
    exit 1
fi

# Doc moi dong trong 'monhoc.txt'
while IFS= read -r class_code; do
    # Tao thu muc ma lop ben trong thu muc ten nguoi dung
    mkdir "$name/$class_code"
    # Kiem tra xem thu muc da duoc tao thanh cong chua
    if [ $? -eq 0 ]; then
        echo "Thu muc '$class_code' da duoc tao thanh cong trong '$name'."
    else
        echo "Thu muc '$class_code' da duoc tao that bai ben trong '$name'."
    fi
done < monhoc.txt

echo "Cac thu muc da duoc tao thanh cong."
