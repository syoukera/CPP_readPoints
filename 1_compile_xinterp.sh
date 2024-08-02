#!/bin/bash

# for space interpolation
# g++ read_points.cpp interpolate.cpp writeData.cpp -o read_point_gxx
icpx read_points.cpp interpolate.cpp writeData.cpp -o read_point_icpx -qopenmp
