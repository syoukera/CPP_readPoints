program read_vectors
    implicit none
    integer :: n, i, j, k
    double precision :: x, y, z
    character(len=100) :: line
    character(len=1) :: paren

    integer, parameter :: grid_size = 101
    integer, parameter :: total_grid_size = grid_size*grid_size
    double precision, dimension(grid_size, grid_size, 3) :: gridData

    ! parameter of FK3 grid
    integer, parameter :: nx = 2000, ny = 1100, nz = 1, ibd = 4, jbd = 4, kbd = 4

    integer, parameter :: jinlet_center = ny/2.0;
    integer, parameter :: jinlet_sta = jinlet_center - (grid_size - 1)/2.0
    integer, parameter :: jinlet_end = jinlet_center + (grid_size - 1)/2.0
    integer :: jinlet
    integer, parameter :: kinlet = (grid_size - 1)/2.0 + 1 ! fix reference point of z index on import velocity as center line

    ! assume no mpi
    integer, parameter :: ista = 1, iend = nx
    integer, parameter :: jsta = 1, jend = ny
    integer, parameter :: ksta = 1, kend = nz

    double precision, parameter :: u1 = 1.0 ! air velocity [cm/s]

    double precision, allocatable, save :: u(:,:,:), v(:,:,:), w(:,:,:)

    allocate (u(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))
    allocate (v(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))
    allocate (w(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))
    
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


    ! Asign velocities
    ! for loop among z and y range 
    do k = ksta - kbd, kend + kbd
        do j = jsta - jbd, jend + jbd

            ! for loop inside on upper boundery of x
            do i = ista, ista - ibd , -1

                ! calclate first row, and later copy it
                if (i == ista) then
                    
                    ! check if y is insile of inlet
                    if (jinlet_sta <= j .and. j <= jinlet_end) then

                        ! get j-index as origin to be inlet start
                        jinlet = j - (jinlet_sta - 1)

                        ! grid data is (z, y, x) cordinate
                        u(i, j, k) = gridData(kinlet, jinlet, 3) ! z-direction of import velocity
                        v(i, j, k) = gridData(kinlet, jinlet, 2) ! y-direction of import velocity
                        w(i, j, k) = gridData(kinlet, jinlet, 1) ! x-direction of import velocity

                    else
                        ! set surrounding velocity
                        u(i, j, k) = u1
                    endif
                else 
                    ! set same velocity as first row
                    u(i, j, k) = u(ista, j, k)
                endif

            end do
        end do
    end do

    ! Close the file
    close(10)
    
end program read_vectors
  