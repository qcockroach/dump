
#ifndef diff_02_h
#define diff_02_h

#include <stdio.h>

char*diff_last_modified(const char* filename);
void version(void);
void brief(const char*filename1, const char*filename2);
void identical(const char*filename1,const char* filename2);
void todo_list(void);
void loadfiles(const char* filename1,const char*filename2);
void context(const char* filename1,const char* filename2);
void print_option(const char*name,int value);
void diff_output_conflict_error(void);
void setoption(const char* arg, const char*s, const char*t, int* value);
void showoptions(const char* file1, const char* file2);
void init_options_files(int argc, const char* argv[]);

#endif
