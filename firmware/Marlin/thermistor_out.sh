#!/bin/sh
awk '{printf("  {%d*OVERSAMPLENR, %d},\n",$1,$2);}'
