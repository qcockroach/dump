// Although compilation in clang results in 2 warnings regarding argv assignments, code will compile regardless
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "diff.h"
#define TRUE    1
#define FALSE   0
#define MAXLINE 1000
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"

// Prints version, license, and author 
void version(void);
// Prints help
void help(void);
// Calculates the hamming distance between two strings
int hamming_distance(char str1[], char str2[]);
// Determines whether or not input is a number, specifically only int
bool is_number(char num[]);


int main (int argc, const char * argv[]) {
    // Error: no arguments
    if (argc == 1) {
        fprintf(stderr, "Error: no arguments\nInvoke -h for help\n");
        return 1;
    }


    int counter = argc;
    char * temp;
    int NUM = 0;
    int brief = FALSE, report_identical_files = FALSE, comparison = FALSE, left_column = FALSE, suppress_common_lines = FALSE, copy = FALSE, unified = FALSE;
    char * filename1, * filename2;
    // Cycle through each argument
    while (counter > 1) {
        temp = *++argv;
        // Number is detected
        if (is_number(temp) == true) {
            if (copy == TRUE && unified == TRUE) {
                fprintf(stderr, "Error: problematic (unknown) input\n**Both -c and -u cannot be in use**\n");
                return 1;
            }
            if (copy == FALSE && unified == FALSE) {
                fprintf(stderr, "Error: problematic (unknown) input\n**If specifying a number then -c or -u must be in use**\n");
                return 1;
            }
            NUM = atoi(temp);
        }
        else if (strcmp(temp, "-h") == 0) help();
        else if (strcmp(temp, "-v") == 0) version();
        else if (strcmp(temp, "-q") == 0) brief = TRUE;
        else if (strcmp(temp, "-s") == 0) report_identical_files = TRUE;
        else if (strcmp(temp, "-y") == 0) comparison = TRUE;
        else if (strcmp(temp, "-left-column") == 0 && comparison == TRUE) left_column = TRUE;
        else if (strcmp(temp, "-suppress-common-lines") == 0 && comparison == TRUE) suppress_common_lines = TRUE;
        else if (strcmp(temp, "-c") == 0) copy = TRUE; 
        else if (strcmp(temp, "-u") == 0) unified = TRUE; 
        else {
            filename1 = temp;
            if (counter < 2) {
                fprintf(stderr, "Error: problematic (unknown) input\n**2 files must be specified; a 3rd file will be ignored**\n");
                return 1;
            }
            filename2 = *++argv;
            counter = 1;
        }
        counter--;
    }


    // Terminate if no files are specified
    if (argc == 2) {
        printf("Error: no files have been specified\n");
        return 1;
    }


    FILE * file1, * file2;
    // Checks if files are able to be opened
    if ((file1 = fopen(filename1, "r")) == NULL) {
        fprintf(stderr, "Error: [%s] could not be opened\n", filename1);
        return 1;
    }
    if ((file2 = fopen(filename2, "r")) == NULL) {
        fprintf(stderr, "Error: [%s] could not be opened\n", filename2);
        return 1;
    }

    
    // Store both files into arrays
    char line1[MAXLINE], line2[MAXLINE];
    int file1_linetotal = 1, file2_linetotal = 1;
    char arr1[MAXLINE][MAXLINE];
    char arr2[MAXLINE][MAXLINE];
    strcpy(arr1[0], "n/a");  // arrays start at index 1 instead of 0
    strcpy(arr2[0], "n/a");
    while (fgets(line1, MAXLINE, file1) != NULL) strcpy(arr1[file1_linetotal++], line1);
    while (fgets(line2, MAXLINE, file2) != NULL) strcpy(arr2[file2_linetotal++], line2);
    // Checks if files are different
    int different = FALSE;  // files default at similar
    for (int i = 1; i <= file1_linetotal; i++) {
        if (strcmp(arr1[i], arr2[i]) != 0) different = TRUE;  // files are different;
    }


    // Both -s and -q
    if (report_identical_files == TRUE && brief == TRUE) {
        if (different == FALSE) printf("Files [%s] and [%s] are identical\n", filename1, filename2);
        else if (different == TRUE) printf("Files [%s] and [%s] differ\n", filename1, filename2);
        return 0;
    }


    // Similarities: s
    if (report_identical_files == TRUE) {
        if (different == FALSE) {
            printf("Files [%s] and [%s] are identical\n", filename1, filename2);
            return 0;
        }
    }


    // Brief: -q
    if (brief == TRUE && different == TRUE) printf("Files [%s] and [%s] differ\n", filename1, filename2);


    // Comparison: -y
    if (comparison == TRUE) {
        int file1_linecount = 1;
        int file2_linecount = 1;
        while (file1_linecount <= file1_linetotal && file2_linecount <= file2_linetotal) {
            // If lines are not equal
            if (strcmp(arr1[file1_linecount], arr2[file2_linecount]) != 0) {
                int found = FALSE;
                for (int i = file2_linecount; i <= file2_linetotal; i++) {
                    // Found element can not be a newline
                    if (strcmp(arr1[file1_linecount], arr2[i]) == 0 && strcmp(arr1[file1_linecount], "\n")) {
                        found = i;
                        goto BREAK1; //  break out of loop completely
                    }
                }

                BREAK1:
                // Similar lines with a difference of 1 character
                if (hamming_distance(arr1[file1_linecount], arr2[file2_linecount]) == 1 && found == FALSE) {
                    int length = strlen(arr1[file1_linecount]) - 1; // everything but the newline
                    for (int i = 0; i < length; i++) { printf("%c", arr1[file1_linecount][i]); }
                    // I am assuming every line is of length 48 aka 6 \t's
                    if (length < 48) { for (int i = 0; i < (48 - length); i++) { printf(" "); } }
                    else printf("\t");
                    printf("|\t%s", arr2[file2_linecount]);
                }
                // DELETION: a line present in file 1 is removed in file 2
                else if (found == FALSE) {
                    int length = strlen(arr1[file1_linecount]) - 1; // everything but the newline
                    for (int i = 0; i < length; i++) { printf("%c", arr1[file1_linecount][i]); }
                    // I am assuming every line is of length 48 aka 6 \t's
                    if (length < 48) { for (int i = 0; i < (48 - length); i++) { printf(" "); } }
                    else printf("\t");
                    printf("<\n");
                    // Don't increment line count of file 2
                    file2_linecount--;
                }
                // INSERTION: found an addition in file 2 not present in file 1
                else {
                    int temp = file2_linecount;
                    file2_linecount = found;
                    while (temp < file2_linecount) {
                        printf("\t\t\t\t\t\t");
                        printf(">  \t%s", arr2[temp]);
                        temp++;
                    }
                }
            }
            // If lines are equal
            else if (suppress_common_lines == FALSE) {
                int length = strlen(arr1[file1_linecount]) - 1; // everything but the newline
                for (int i = 0; i < length; i++) { printf("%c", arr1[file1_linecount][i]); }
                // I am assuming every line is of length 48 aka 6 \t's
                if (length < 48) { for (int i = 0; i < (48 - length); i++) { printf(" "); } }
                else printf("\t");
                // Right column options
                if (left_column == FALSE) { printf("\t%s", arr1[file1_linecount]); }
                else { printf("(\n"); }
            }
            file1_linecount++;
            file2_linecount++;
        }
        
        fclose(file1);
        fclose(file2);

        return 0;
    }

    // Copied text: -c
    if (copy == TRUE) {
        // NUM defaults at 3
        if (NUM == 0) NUM = 3;
        int file1_linecount = 1;
        int file2_linecount = 1;
        int i = 0;
        int change = 0;
        while (file1_linecount <= file1_linetotal && file2_linecount <= file2_linetotal && i < NUM) {
            // If lines are not equal
            if (strcmp(arr1[file1_linecount], arr2[file2_linecount]) != 0) {
                int found = FALSE;
                for (int i = file2_linecount; i <= file2_linetotal; i++) {
                    // Found element can not be a newline
                    if (strcmp(arr1[file1_linecount], arr2[i]) == 0 && strcmp(arr1[file1_linecount], "\n")) {
                        found = i;
                        goto BREAK3; //  break out of loop completely
                    }
                }

                BREAK3:
                // DELETION: a line present in file 1 is removed in file 2
                if (found == FALSE) {
                    printf("!  %s", arr1[file1_linecount]);
                    // Don't increment line count of file 2
                    file2_linecount--;
                    change++;
                }
            }
            // Lines are equal
            else printf("   %s", arr1[file1_linecount]);
            file1_linecount++;
            file2_linecount++;
            i++;
        }

        printf("\n\n--- %d changes in %d lines ---\n\n", change, NUM);

        file1_linecount = 1;
        file2_linecount = 1;
        i = 0;
        change = 0;
        while (file1_linecount <= file1_linetotal && file2_linecount <= file2_linetotal && i < NUM) {
            // If lines are not equal
            if (strcmp(arr1[file1_linecount], arr2[file2_linecount]) != 0) {
                int found = FALSE;
                for (int i = file2_linecount; i <= file2_linetotal; i++) {
                    // Found element can not be a newline
                    if (strcmp(arr2[file2_linecount], arr1[i]) == 0 && strcmp(arr2[file2_linecount], "\n")) {
                        found = i;
                        goto BREAK4; //  break out of loop completely
                    }
                }

                BREAK4:
                // DELETION: a line present in file 1 is removed in file 2
                if (found == FALSE) {
                    printf("!  %s", arr2[file2_linecount]);
                    // Don't increment line count of file 2
                    file1_linecount--;
                    change++;
                }
            }
            // Lines are equal
            else printf("   %s", arr2[file2_linecount]);
            file1_linecount++;
            file2_linecount++;
            i++;
        }
        printf("\n\n--- %d changes in %d lines ---\n\n", change, NUM);

        fclose(file1);
        fclose(file2);

        return 0;
    }

    
    // Unified text: -u
    if (unified == TRUE) {
        // NUM defaults at 3
        if (NUM == 0) NUM = 3;
        int file1_linecount = 1;
        int file2_linecount = 1;
        int i = 0;
        int change = 0;
        while (file1_linecount <= file1_linetotal && file2_linecount <= file2_linetotal && i < NUM) {
            // If lines are not equal
            if (strcmp(arr1[file1_linecount], arr2[file2_linecount]) != 0) {
                int found = FALSE;
                for (int i = file2_linecount; i <= file2_linetotal; i++) {
                    // Found element can not be a newline
                    if (strcmp(arr1[file1_linecount], arr2[i]) == 0 && strcmp(arr1[file1_linecount], "\n")) {
                        found = i;
                        goto BREAK5; //  break out of loop completely
                    }
                }

                BREAK5:
                // DELETION: a line present in file 1 is removed in file 2
                if (found == FALSE) {
                    printf("-  %s", arr1[file1_linecount]);
                    // Don't increment line count of file 2
                    file2_linecount--;
                    change++;
                }
                // INSERTION: found an addition in file 2 not present in file 1
                else {
                    int temp = file2_linecount;
                    file2_linecount = found;
                    while (temp < file2_linecount && i < NUM) {
                        printf("+  %s", arr2[temp]);
                        temp++;
                        i++;
                        change++;
                    }
                }
            }
            // Lines are equal
            else printf("   %s", arr1[file1_linecount]);
            file1_linecount++;
            file2_linecount++;
            i++;
        }
        printf("\n--- %d changes in %d lines ---\n", change, NUM);

        fclose(file1);
        fclose(file2);

        return 0;
    }
    
    
    // Default
    int file1_linecount = 1;
    int file2_linecount = 1;
    while (file1_linecount <= file1_linetotal && file2_linecount <= file2_linetotal) {
        // If lines are not equal
        if (strcmp(arr1[file1_linecount], arr2[file2_linecount]) != 0) {
            int found = FALSE;
            for (int i = file2_linecount; i <= file2_linetotal; i++) {
                // Found element can not be a newline
                if (strcmp(arr1[file1_linecount], arr2[i]) == 0 && strcmp(arr1[file1_linecount], "\n")) {
                    found = i;
                    goto BREAK2; //  break out of loop completely
                }
            }

            BREAK2:
            // DELETION: a line present in file 1 is removed in file 2
            if (found == FALSE) {
                printf("%s-  line %d\t:\t%s", RED, file1_linecount, arr1[file1_linecount]);
                // Don't increment line count of file 2
                file2_linecount--;
            }
            // INSERTION: found an addition in file 2 not present in file 1
            else {
                int temp = file2_linecount;
                file2_linecount = found;
                while (temp < file2_linecount) {
                    printf("%s+  line %d\t:\t%s", GREEN, temp, arr2[temp]);
                    temp++;
                }
            }
        }
        file1_linecount++;
        file2_linecount++;
    }

    fclose(file1);
    fclose(file2);

    return 0;
}
