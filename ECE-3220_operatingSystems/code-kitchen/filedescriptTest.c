#include <sys/time.h>
#include <sys/types.h>

int pd[2];

myprocess()
{
  char str[20];
  int ret=0;
  struct timeval tv;

  fd_set fds;
  tv. tv_sec = 0;
  tv. tv_usec = 0;


  while ( ret == 0 ) { 
    FD_ZERO( &fds ) ;
    FD_SET( pd[0], &fds );
    ret = select( pd[0]+1, &fds, (fd_set *)0, (fd_set *)0, &tv );
   }

  read(pd[0], str, 20);
}