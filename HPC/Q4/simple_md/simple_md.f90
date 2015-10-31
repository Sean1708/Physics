! -*- mode: F90 ; mode: font-lock ; column-number-mode: true -*-
!=============================================================================!
!                          M D _ S T U F F                                    !
!=============================================================================!
!                                                                             !
! $Id:
!                                                                             !
!-----------------------------------------------------------------------------!
! Simple container module for MD data and routines for operating thereupon,   !
! e.g. force calculation and position updating                                !
!-----------------------------------------------------------------------------!
! Written by Matt Probert, v2.0, 14/09/2010                                   !
!-----------------------------------------------------------------------------!
module md_stuff
  implicit none

  !Simulation parameters
  integer, parameter :: ndim=3           ! dimensionality of the physical space
  integer, parameter :: nparts=10000     ! number of particles
  integer, parameter :: nsteps=2         ! number of time steps in the simulation
  integer, parameter :: dp=selected_real_kind(15,300)
  logical, parameter :: fixed_seed=.true.   ! T => repeatable rands for testing 
  real(kind=dp), parameter :: mass=1.0_dp   ! mass of the particles
  real(kind=dp), parameter :: dt=1.0E-4_dp  ! time step

  !Simulation variables
  real(kind=dp), save, dimension(1:ndim)          :: box=(/10.0_dp,10.0_dp,10.0_dp/)
  real(kind=dp), save, dimension(1:ndim,1:nparts) :: pos
  real(kind=dp), save, dimension(1:ndim,1:nparts) :: vel
  real(kind=dp), save, dimension(1:ndim,1:nparts) :: force
  real(kind=dp), save, dimension(1:ndim,1:nparts) :: accel
  real(kind=dp), save                             :: PE, KE

contains

  subroutine init
    !=========================================================================!
    ! Initialise positions, velocities and accelerations of all particles.    !
    ! Use system random number generator and either fixed or random seed.     !
    !-------------------------------------------------------------------------!
    ! References:                                                             !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Arguments:                                                              !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Return value:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Parent module variables used:                                           !
    !   pos, vel, accel all updated.                                          !
    !   fixed_seed: if set to .true. then get a repeatable sequence of rands  !
    !-------------------------------------------------------------------------!
    ! Modules used:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Key Internal Variables:                                                 !
    !   rand_seed: set to clock-based value if fixed_seed=.false.             !
    !-------------------------------------------------------------------------!
    ! Necessary conditions:                                                   !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Written by Matt Probert, v1.0, 14/02/2005                               !
    !=========================================================================!
    implicit none

    !random number stuff
    integer :: rand_size
    integer, allocatable, dimension(:) :: rand_seed

    !f90 intrinsic time call
    character(len=10) :: system_time           !length is crucial ...
    real (kind=dp)    :: rtime
    integer :: i,ierr

    !random numbers for positions
    call random_seed(size=rand_size)
    allocate(rand_seed(1:rand_size),stat=ierr)
    if (ierr/=0) stop 'Error in allocating rand_seed'
    if (fixed_seed) then
       do i=1,rand_size
          rand_seed(i)=223257167+i
       end do
    else
       call date_and_time(time=system_time)  !character string hhmmss.xxx
       read (system_time,*) rtime            !convert to real
       rand_seed = int(rtime * 1000.0_dp)    !0<rtime<235959999.0 which fits within huge(1)
    end if
    call random_seed(put=rand_seed)
    deallocate(rand_seed,stat=ierr)
    if (ierr/=0) stop 'Error in deallocating rand_seed'
    !now random number generator is ready to use ...

    ! set initial positions and velocities
    call random_number(pos)                  !0<random_number<1
    do i=1,ndim
       pos(i,:)=pos(i,:)*box(i)
    end do
    vel(:,:) = 0.0_dp
    accel(:,:) = 0.0_dp

    return
  end subroutine init

  subroutine compute
    !=========================================================================!
    ! Compute the forces on the particles using a simple model potential:     !
    !   V(dr)=sin(dr)^2 if dr<pi/2, 1.0 otherwise                             !
    !   where dr=|r_i-r_j| separation of particles i and j                    !
    !   Hence dV=2*sin(dr)*cos(dr) if dr<pi/2, 0.0 otherwise                  !
    !-------------------------------------------------------------------------!
    ! References:                                                             !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Arguments:                                                              !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Return value:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Parent module variables used:                                           !
    !   pos unchanged, force, PE and KE updated                               !
    !-------------------------------------------------------------------------!
    ! Modules used:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Key Internal Variables:                                                 !
    !   v(x) and dv(x) are statement functions                                !
    !-------------------------------------------------------------------------!
    ! Necessary conditions:                                                   !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Written by Matt Probert, v1.0, 14/02/2005                               !
    !=========================================================================!
    implicit none

    !local stuff
    integer :: i, j
    real(kind=dp), dimension(1:ndim) :: rij
    real(kind=dp)  :: d

    PE = 0.0_dp
    KE = 0.0_dp

    do i=1,nparts
       !compute potential energy and forces
       force(:,i) = 0.0_dp
       do j=1,nparts

          if (i/=j) then
             rij(:)=pos(:,i)-pos(:,j)
             d=sqrt(dot_product(rij,rij))
             ! attribute half of the potential energy to particle 'j'
             PE=PE+0.5_dp*V(d)
             force(:,i)=force(:,i)-rij(:)*dV(d)/d
          end if
       enddo
       ! compute kinetic energy
       KE=KE+dot_product(vel(:,i),vel(:,i))
    enddo
    KE=KE*0.5_dp*mass

    return
  end subroutine compute

  real(kind=dp) function V(x)
    !Define the potential as a harmonic well which smoothly saturates to a
    !maximum value at PI/2
    implicit none
    real (kind=dp), intent(in) :: x
    real(kind=dp), parameter :: pi=3.141592653589793238462643383279502884197_dp
    real(kind=dp), parameter :: pi_2=pi/2.0_dp

    V=sin(min(x,pi_2))**2

    return
  end function V

  real(kind=dp) function dV(x)
    !Define the derivative of the potential
    implicit none
    real (kind=dp), intent(in) :: x
    real(kind=dp), parameter :: pi=3.141592653589793238462643383279502884197_dp
    real(kind=dp), parameter :: pi_2=pi/2.0_dp

    dV=2.0_dp*sin(min(x,pi_2))*cos(min(x,pi_2))
    
    return
  end function dV

  subroutine update
    !=========================================================================!
    ! Simple Velocity-Verlet MD update of positions                           !
    ! i.e. r(t+dt) = r(t) + v(t)*dt + f(t)*dt**2/(2*m) +O(dt**4)              !
    !      v(t+dt) = v(t) + ( f(t+dt)+f(t) )*dt/(2*m)  +O(dt**3)              !
    !-------------------------------------------------------------------------!
    ! References:                                                             !
    !   Swope et al, J.Chem.Phys. v75, p637-649 (1982)                        !
    !-------------------------------------------------------------------------!
    ! Arguments:                                                              !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Return value:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Parent module variables used:                                           !
    !   pos, vel and force used,                                              !
    !   pos, vel and accel updated                                            !
    !-------------------------------------------------------------------------!
    ! Modules used:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Key Internal Variables:                                                 !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Necessary conditions:                                                   !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Written by Matt Probert, v1.0, 14/02/2005                               !
    !=========================================================================!
    implicit none

    !local stuff
    integer i, j

    do i = 1,nparts
       do j = 1,ndim
          pos(j,i)=pos(j,i)+vel(j,i)*dt+0.5_dp*dt*dt*accel(j,i)
          vel(j,i)=vel(j,i) +0.5_dp*dt*force(j,i)/mass+0.5_dp*dt*accel(j,i)
          accel(j,i)=force(j,i)/mass
       enddo
    enddo

    return
  end subroutine update

end module md_stuff

program md
  !=========================================================================!
  ! Simple example of MD using simple model potential                       !
  ! and Velocity-Verlet integration of equations of motion.                 !
  !-------------------------------------------------------------------------!
  ! References:                                                             !
  !   Swope et al, J.Chem.Phys. v75, p637-649 (1982)                        !
  !-------------------------------------------------------------------------!
  ! Arguments:                                                              !
  !   none                                                                  !
  !-------------------------------------------------------------------------!
  ! Return value:                                                           !
  !   none                                                                  !
  !-------------------------------------------------------------------------!
  ! Parent module variables used:                                           !
  !   none                                                                  !
  !-------------------------------------------------------------------------!
  ! Modules used:                                                           !
  !   md_stuff                                                              !
  !-------------------------------------------------------------------------!
  ! Key Internal Variables:                                                 !
  !   none                                                                  !
  !-------------------------------------------------------------------------!
  ! Necessary conditions:                                                   !
  !   none                                                                  !
  !-------------------------------------------------------------------------!
  ! Written by Matt Probert, v1.0, 14/02/2005                               !
  !=========================================================================!
  use md_stuff
  implicit none
  integer :: i, ierr, log_unit
  real(kind=dp) :: Etot

  !setup random initial configuration of positions etc
  call init

  !compute initial forces and energies
  call compute

  !store initial energy so can easily monitor drift
  Etot=PE+KE

  !set up monitoring output file
  log_unit=10
  open(unit=log_unit,file="simple_md.log",form="formatted", &
       & status="replace",iostat=ierr)
  if (ierr/=0) stop 'Error in initialising simple_md.log'
  write(log_unit,'(1x,3A20)') "PE","KE","Error"
  close(unit=log_unit,iostat=ierr)
  if (ierr/=0) stop 'Error in closing simple_md.log'

  !now loop over MD time steps
  do i=1,nsteps

     !calculate new forces and energies
     call compute

     !write out energies to logfile
     open(unit=log_unit,file="simple_md.log",form="formatted", &
          & status="old",position="append",iostat=ierr)
     if (ierr==0) then
        write(log_unit,'(1x,3E20.10)') PE,KE,(PE+KE-Etot)/Etot
     else
        stop 'Error in opening simple_md.log'
     end if
     close(unit=log_unit,iostat=ierr)
     if (ierr/=0) stop 'Error in closing simple_md.log'

     !perform MD update using NVE ensemble
     call update

  end do

end program md












