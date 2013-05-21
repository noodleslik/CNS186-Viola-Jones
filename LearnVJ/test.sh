#!/bin/bash

data="./2000.txt"
image_path="../WebFaces"


image_list=`ls $image_path`


for image in $image_list
do
	./mshcd $image_path/"$image" $data
	read a
done

