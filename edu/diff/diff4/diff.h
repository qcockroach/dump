#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// Prints version, license, and author 
void version(void) {
    printf("\ndiff (CSUF diffutils) 1.0.0\n");
    printf("Copyright (C) 2014 CSUF\n");
    printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
    printf("You may redistribute copies of this program\n");
    printf("under the terms of the GNU General Public License.\n");
    printf("For more information about these matters, see the file named COPYING.\n");
    printf("Written by Tianzheng Li\n\n");
}

// Prints help
void help(void) {
    printf("-h                      :   help\n");
    printf("-v                      :   prints version, license, and author\n");
    printf("-q                      :   brief (reports only whether files are different; silent if they are not)\n");
    printf("-s                      :   report-identical-files (reports if files are the same; usual out if they are not)\n");
    printf("-y                      :   comparison (prints common paragraphs)\n");
    printf("                            ** [ < ] , [ > ] , and [ | ] represent removal, addition and similarity respectively**\n");
    printf("-left-column            :   if -y is used, then prints only the left column of common lines\n");
    printf("                            ** [ ( ] is used to represent common lines**\n");
    printf("-suppress-common-lines  :   if -y is used, then common lines are not shown\n");
    printf("[filename]              :   two given files are compared (requires two file names, one followed by another)\n");
    printf("                            **The two files must be the two last elements in the argument**\n");
    printf("-c                      :   shows a default of 3 lines of copied text\n");
    printf("                            **A number following will determine how many lines (enter just the number)**\n");
    printf("-u                      :   shows a default of 3 lines of unified text\n");
    printf("                            **A number following will determine how many lines (enter just the number)**\n");
}

// Calculates the hamming distance between two strings
int hamming_distance(char str1[], char str2[]) {
    int i = 0;
    int counter = 0;
    while (str1[i] != '\0') {
        if (str1[i] != str2[i]) counter++;
        i++;
    }
    return counter;
}

// Determines whether or not input is a number, specifically only int
bool is_number(char num[]) {
    bool b = true;
    int length = strlen(num);
    for (int i = 0; i < length; i++) {
        if (isdigit(num[i]) == 0) b = false; 
    }
    return b;
}


