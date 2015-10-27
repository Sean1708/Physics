program test_inv
implicit none
integer, parameter :: precision=kind(1.0)
real (kind=precision) :: x,y,z
integer :: i, found

x=0
found=0
do i=1,1000
   x=x+1.0
   y=1.0/x
   z=y*x
   if (z /= 1.0) then
      print *,'failed with x=',x,'y=',y
      found=found+1
   end if
end do
print *,'Found ',found
end program test_inv
