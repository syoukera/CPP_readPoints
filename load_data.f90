program read_vectors
    implicit none
    integer :: n, i
    real :: x, y, z
    character(len=100) :: line
    character(len=1) :: paren
    
    ! Open the file
    open(unit=10, file='output/0.0000000/U', status='old', action='read')
    
    ! Read the number of vectors
    read(10, *) n
    print *, 'Number of vectors:', n
  
    ! Skip the line with '('
    read(10, '(A)') line
    
    ! Read each vector
    do i = 1, n
      read(10, '(A)') line  ! Read the line into a string
      read(line, '(A1, F9.7, 1X, F9.7, 1X, F9.7, A1)') paren, x, y, z, paren  ! Parse the string
      print *, 'Vector ', i, ': ', x, y, z
    end do
    
    ! Close the file
    close(10)
    
end program read_vectors
  