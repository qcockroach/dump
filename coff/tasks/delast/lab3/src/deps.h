#ifndef __DEPS_H
#define __DEPS_H

# ifdef __WIN32
# include <windows.h>
# include <conio.h>
#  define KB_ENTER   	13

# else
#  include <unistd.h>
#  include <termios.h>
#  include <sys/ioctl.h>
#  define KB_ENTER 		10

int putch (int c);
int getch (void);
int kbhit (void);
# endif

#define KB_ESCAPE      	27

#endif
