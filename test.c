#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>//fork
#include <fcntl.h>
#include <stdbool.h>

#define OUTPUT_FILENAME "test.out"

//
// Created by Anchugov Roman on 16/01/2020.
//
int main() {
    int fd = open(OUTPUT_FILENAME, O_WRONLY | O_CREAT, 0600);//open or create file for output
    if (fork() == 0) {//Child
        printf("Child\n");

        dup2(fd, STDOUT_FILENO);//redirect std output to file

        printf("out in file ollo \n");
        sleep(1);

    } else {//Parent
        wait(NULL);//need link to an int that will contain status
        printf("\nParent\n");

        FILE *fptr;
        if ((fptr = fopen(OUTPUT_FILENAME, "r")) == NULL) {//check for error during opening of "output" file
            printf("\nFile opening or doesnt exist error\n");
            exit(1);
        }

        int symbol = fgetc(fptr);//take text from  file symbol by symbol
        while (symbol != EOF) {
            printf("%c", symbol);
            symbol = fgetc(fptr);
        }
        fclose(fptr);

        printf("\nEOF---\n");

        while (getchar() != 'q') // infitite cycle to catch KeyboardInterrupt signals
        {
        }

        close(fd);
        printf("PROGRAMM FINISHED\n");
    }
}

