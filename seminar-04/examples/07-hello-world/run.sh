#!/usr/bin/env bash

gcc -S -o main.s main.c && cat main.s && rm -f main.s