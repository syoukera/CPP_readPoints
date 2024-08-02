#!/bin/bash

# for time interpolation
# g++ interpolate_time.cpp writeData.cpp -o tinterp_gxx
icpx interpolate_time.cpp writeData.cpp -o tinterp_icpx
# mpiicpc -cxx=icpx interpolate_time.cpp writeData.cpp -o tinterp_mpiicpc