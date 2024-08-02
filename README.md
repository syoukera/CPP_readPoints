# CPP_readPoints

C++ code for reading raw data on turbulent inlet velocity.  
Prepare shaped data to use in the stagard grid and interpolate in XYZ and time.  
  
Grid scale: length 1 mm x 1 mm, dx = 1e-2 mm  
Time scale: dt = 1e-7 s  

## 1_xinterp
Space interpolation from reference velocity data.

```bash
bash 1_compile_xinterp.sh
./read_point_icpx
```

## 2_tinterp
Time interpolation from spacial interpolated data.

```bash
bash 2_compile_tinterp.sh
./tinterp_icpx
```

## 3_load
loading interpolated files.  
Fortran90 code will used for load.

```bash
bash 3_compile_load.sh
./load_data_gfortran
```