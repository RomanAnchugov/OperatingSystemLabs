#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>//fork
#include <fcntl.h>
#include <signal.h>
#include <string.h>

/*
 *Написать программу, предоставляющую информацию о содержи-мом оче¬реди на печать.
 * Передать эту информацию через файловую систему в па¬раллельный процесс
 * и в нем вывести на экран дисплея содержимое тех файлов, которые хотят распечатать.
 * Предусмотреть возможность неод¬нократного прерывания от клавиатуры.
 * При по-ступлении 3-его прерыва¬ния вывести количество файлов в очереди на печать.
 */

#define OUTPUT_FILENAME "lab.out"

#define ERROR_OPEN_FILE_PARENT 1
#define ERROR_OPEN_FILE_CHILD 2

int interrupt_counter = 0;
int files_counter = 0;

void interruption_handler();

int main() {

    test
    another
    ???
    //interruption handler
    struct sigaction keyboard_interrupter;
    keyboard_interrupter.sa_handler = interruption_handler;//make out function as a handler of interruption
    sigemptyset(&keyboard_interrupter.sa_mask);          // no additional blocking signals
    sigprocmask(0, 0, &keyboard_interrupter.sa_mask);      // save current mask
    keyboard_interrupter.sa_flags = 0;                   // not to perform special actions
    sigaction(SIGINT, &keyboard_interrupter, 0);

    int fd = open(OUTPUT_FILENAME, O_WRONLY | O_CREAT, 0600);//open or create file for output

    if (fork() == 0) {//Child
        printf("-Child-\n");

        if (fd == -1) {
            printf("\nFile opening/creating error\n");
            exit(ERROR_OPEN_FILE_PARENT);
        }

//        dup2(fd, STDOUT_FILENO);//redirect std output to file
        execl("/usr/bin/lpq", "lpq");//third argument is parameters without -
    } else {//Parent
        wait(NULL);//link to an int that will contain status
        printf("\n-Parent-\n");

        FILE *fptr;
        if ((fptr = fopen(OUTPUT_FILENAME, "r")) == NULL) {//check for error during opening of "output" file
            printf("\nFile opening or doesnt exist error\n");
            exit(ERROR_OPEN_FILE_CHILD);
        }

        char buff[255];
        char filename[100];

        //skip two first lines with fields information
        fgets(buff, 255, fptr);
        fgets(buff, 255, fptr);

        while (fscanf(fptr, "%s", buff) != EOF) {

            //skip first 3 fields(status, user, task)
            fscanf(fptr, "%s", buff);
            fscanf(fptr, "%s", buff);

            //our name
            files_counter++;
            fscanf(fptr, "%s", filename);
            printf("Files %s content:\n\n", filename);

            //cat to print content of file
            char result_command[200] = "cat ";
            strcat(result_command, filename);
            system(result_command);

            printf("---\n");

            //skip last two fields(size)
            fscanf(fptr, "%s", buff);
            fscanf(fptr, "%s", buff);
        }

        printf("\n-end of operation-\n");

        while (getchar() != 'q') {} // infitite cycle to catch KeyboardInterrupt signals

        //close files
        fclose(fptr);
        close(fd);

        printf("\n-program finished-\n");
    }
}

void interruption_handler() {
    printf("\n");
    interrupt_counter++;
    if (interrupt_counter == 3) {
        printf("Files in lpq %i\n", files_counter);
        interrupt_counter = 0;
    }
}