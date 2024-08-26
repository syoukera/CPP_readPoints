module parameters
    implicit none

    ! parameter of FK3 grid
    integer, parameter :: nx = 1, ny = 1100, nz = 1100, ibd = 4, jbd = 4, kbd = 4
    
    ! assume no mpi
    integer, parameter :: ista = 1, iend = nx
    integer, parameter :: jsta = 1, jend = ny
    integer, parameter :: ksta = 1, kend = nz

    double precision, parameter :: u1 = 1.0 ! air velocity [cm/s]
    double precision, allocatable, save :: u(:,:,:), v(:,:,:), w(:,:,:)

    ! parameter for load data
    integer, parameter :: grid_size = 101
    double precision, dimension(grid_size, grid_size, 3) :: inlet_velocities

end module parameters

subroutine load_U(index_time)
    use parameters
    implicit none

    integer, intent(in) :: index_time
    double precision, parameter :: dt = 1d-7
    double precision :: time

    integer :: n, i, j
    double precision :: x, y, z
    
    character(len=100) :: line, clean_line
    character(len=9) :: name_folder
    character(len=100) :: path_folder

    ! convert to real
    time = index_time - 1
    time = time*1d-7

    ! convert to character
    write(name_folder, "(F9.7)") time
    path_folder = '/NAS/18/NH3_HiTAC/velocity_interpolate/fuel_tinterp//'//name_folder//'/U'
    print *, path_folder, ' will be loaded.'

    ! Open the file
    open(unit=10, file=path_folder, status='old', action='read')
    ! open(unit=10, file='output/0.0000000/U', status='old', action='read')
    
    ! Read the number of vectors
    read(10, *) n
    print *, 'Number of vectors:', n
  
    ! Skip the line with '('
    read(10, '(A)') line
    
    ! Read each vector
    do i = 1, grid_size
        do j = 1, grid_size
            read(10, '(A)') line  ! Read the line into a string

            ! Remove the parentheses
            clean_line = adjustl(line)
            clean_line = clean_line(2:)   ! Remove the first character "("
            clean_line = clean_line(:len_trim(clean_line)-1)  ! Remove the last character ")"

            ! Read the components from the cleaned line
            read(clean_line, *) x, y, z

            inlet_velocities(i, j, 1) = x
            inlet_velocities(i, j, 2) = y
            inlet_velocities(i, j, 3) = z
        end do
    end do

    ! Close the file
    close(10)

    ! Open the file
    open(unit=12, file='output/loaded_U', status='replace', action='write')
    
    ! Print each vector
    do i = 1, grid_size

        write (12, *) (inlet_velocities(i, j, 3), j = 1, grid_size)

    end do
    
    close(12)

end subroutine load_U

program read_vectors
    use parameters
    implicit none

    integer :: n, i, j, k

    integer, parameter :: index_time = 1

    integer, parameter :: jinlet_center = ny/2.0;
    integer, parameter :: jinlet_sta = jinlet_center - (grid_size - 1)/2.0
    integer, parameter :: jinlet_end = jinlet_center + (grid_size - 1)/2.0
    integer :: jinlet
    
    integer, parameter :: kinlet_center = nz/2.0;
    integer, parameter :: kinlet_sta = kinlet_center - (grid_size - 1)/2.0
    integer, parameter :: kinlet_end = kinlet_center + (grid_size - 1)/2.0
    integer :: kinlet

    allocate (u(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))
    allocate (v(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))
    allocate (w(ista-ibd:iend+ibd,jsta-jbd:jend+jbd,ksta-kbd:kend+kbd))

    call load_U(index_time)

    ! Asign velocities
    ! for loop among z and y range 
    do k = ksta - kbd, kend + kbd
        do j = jsta - jbd, jend + jbd

            ! for loop inside on upper boundery of x
            do i = ista, ista - ibd , -1

                ! check if y is insile of inlet
                if (jinlet_sta <= j .and. j <= jinlet_end) then
                    ! check if z is insile of inlet
                    if (kinlet_sta <= k .and. k <= kinlet_end) then

                        ! calclate first row, and later copy it
                        if (i == ista) then

                            ! get j-index as origin to be inlet start
                            jinlet = j - (jinlet_sta - 1)
                            
                            ! get k-index as origin to be inlet start
                            kinlet = k - (kinlet_sta - 1)

                            ! if non-zero z velocity
                            if (inlet_velocities(kinlet, jinlet, 3) .ne. 0.0d0) then

                                ! grid data is (z, y, x) cordinate
                                u(i, j, k) = inlet_velocities(kinlet, jinlet, 3) ! z-direction of import velocity
                                v(i, j, k) = inlet_velocities(kinlet, jinlet, 2) ! y-direction of import velocity
                                w(i, j, k) = inlet_velocities(kinlet, jinlet, 1) ! x-direction of import velocity
                            
                            else ! if loaded velocity is 0.0 

                                ! set surrounding velocity
                                u(i, j, k) = u1
                                v(i, j, k) = 0.0d0
                                w(i, j, k) = 0.0d0

                            endif
                        
                        else ! inside of second layer of x-boundary

                            ! set same velocity as first row
                            u(i, j, k) = u(ista, j, k)
                            v(i, j, k) = v(ista, j, k)
                            w(i, j, k) = w(ista, j, k)

                        endif

                    else ! z is outside of inlet radious
    
                        ! set surrounding velocity
                        u(i, j, k) = u1
                        v(i, j, k) = 0.0d0
                        w(i, j, k) = 0.0d0
    
                    endif

                else ! y is outside of inlet radious

                    ! set surrounding velocity
                    u(i, j, k) = u1
                    v(i, j, k) = 0.0d0
                    w(i, j, k) = 0.0d0

                endif

            end do
        end do
    end do

    ! Open the file
    open(unit=11, file='output/asigned_U', status='replace', action='write')

    i = ista ! set x as first layer on boundary

    do k = ksta - kbd, kend + kbd

        write(11, *) (u(i, j, k), j = jsta - jbd, jend + jbd)

    end do

    close(11)

end program read_vectors
  