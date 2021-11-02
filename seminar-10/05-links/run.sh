#!/bin/zsh

(rm x.txt ; rm x_hard.txt ; rm x_sym.txt ; rm x_ordinary.txt) 2> /dev/null
touch x_ordinary.txt
touch x.txt
link x.txt x_hard.txt
ln -s x.txt ./x_sym.txt
ls -la x*
(rm x.txt ; rm x_hard.txt ; rm x_sym.txt ; rm x_ordinary.txt) 2> /dev/null