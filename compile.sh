#!/bin/bash

# for space interpolation
# g++ read_points.cpp interpolate.cpp writeData.cpp -o read_point_gxx
# icpx read_points.cpp interpolate.cpp writeData.cpp -o read_point_icpx -qopenmp

# for time interpolation
# g++ interpolate_time.cpp writeData.cpp -o tinterp_gxx
icpx interpolate_time.cpp writeData.cpp -o tinterp_icpx
# mpiicpc -cxx=icpx interpolate_time.cpp writeData.cpp -o tinterp_mpiicpc