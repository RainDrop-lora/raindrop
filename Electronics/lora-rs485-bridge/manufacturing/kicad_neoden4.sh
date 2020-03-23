#!/bin/bash

#Partly based on http://kicad-neoden.blogspot.com/2017/11/kicad-position-file.html

#Replace 270 in Rot column with -90 --> i.e. transform the coordinates
#Move Footprint to second column
#Copy  PosX, PosY to 3rd column
#Rename 3,4 columns to Mid X, Mid Y
#Copy PosX, PosY to 5th, 6th column
#Rename 5th, 6th columns to Ref X, Ref Y
#Copy PosX, PosY to 7th, 8th column
#Rename 5th, 6th columns to Pad X, Pad Y
#Rename Side to Layer
#Rename Rot to Rotation
#Move Rotation column to 9th column
#Move Layer to 10th column
#Rename Val to Comment
#Move Comment to 11th column
#Replace top with T


[ $# -ne 1 ] && { echo "Usage: $0 <input file> <output file>"; exit 999; }

sed '1d;s/top/T/g' $1 | awk -F, ' BEGIN {OFS=","; print "Designator,Footprint,Mid X,Mid Y,Ref X,Ref Y,Pad X,Pad Y,Layer,Rotation,Comment" } 
{print $1, $3, $4, $5, $4, $5, $4, $5, $7, ($6>180)? -1*(360-$6) : $6, $2 }'





