program read_vectors
    implicit none
    integer :: n, i, j
    real :: x, y, z
    character(len=100) :: line
    character(len=1) :: paren

    integer, parameter :: grid_size = 101
    integer, parameter :: total_grid_size = grid_size*grid_size
    real, dimension(grid_size, grid_size, 3) :: gridData
    
    ! Open the file
    open(unit=10, file='output/0.0000000/U', status='old', action='read')
    
    ! Read the number of vectors
    read(10, *) n
    print *, 'Number of vectors:', n
  
    ! Skip the line with '('
    read(10, '(A)') line
    
    ! Read each vector
    do i = 1, grid_size
        do j = 1, grid_size
            read(10, '(A)') line  ! Read the line into a string
            read(line, '(A1, F9.7, 1X, F9.7, 1X, F9.7, A1)') paren, x, y, z, paren  ! Parse the string
            ! print *, 'Vector ', i, ", ", j, ': ', x, y, z

            gridData(i, j, 1) = x
            gridData(i, j, 2) = y
            gridData(i, j, 3) = z
        end do
    end do

    
    ! Read each vector
    do i = 1, grid_size
        do j = 1, grid_size

            print *, 'Vector ', i, ", ", j, ': ', gridData(i, j, 1), gridData(i, j, 2), gridData(i, j, 3)

        end do
    end do
    
    ! Close the file
    close(10)
    
end program read_vectors
  