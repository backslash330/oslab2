


// Name your programs as indicated below. Your executable should be named “ChocolateFactory”.
// Do not submit programs with executables named a.out! 

// Charlie (charlie.c)
// Your main function will be responsible for the overall control of the factory: creating the assembly
// line (bounded-buffer array), spawning o oompa loompa, and c child workers (threads). Once all
// of the oompa loompa's have finished producing candies, you must wait for all the candies
// remaining in the assembly line to get packaged then let the children go home for the day.
// nalmeida@Desktop-Nick:~/os/lab2$ gcc charlie.c -o ChocolateFactory
// nalmeida@Desktop-Nick:~/os/lab2$ ./ChocolateFactory 1 2 3 4 5
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "ChocolateFactory.h"


int main(int argc, char *argv[]) {
    // Check that there are 5 arguments, if not, print an error message and exit
    if (argc != 6) {
        printf("Error: 5 arguments required\n");
        exit(1);
    }

    // If any of the 5 arguments are not ints, print an error message and exit
    for (int i = 1; i < 6; i++) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (argv[i][j] < '0' || argv[i][j] > '9') {
                printf("Error: All arguments must be integers\n");
                exit(1);
            }
        }
    }
    
    // Create a factory with the given arguments
    ChocolateFactory *factory = malloc(1 * sizeof(ChocolateFactory));
    factory->oompa_loompas = atoi(argv[1]);
    factory->children = atoi(argv[2]);
    factory->assembly_line = atoi(argv[3]);
    factory->candies_per_box = atoi(argv[4]);
    factory->candies_per_oompa = atoi(argv[5]);

    // print the factory settings
    printf("Factory Settings:\n");
    printf("Oompa Loompas: %d\n", factory->oompa_loompas);
    printf("Children: %d\n", factory->children);
    printf("Assembly Line: %d\n", factory->assembly_line);
    printf("Candies per Box: %d\n", factory->candies_per_box);
    printf("Candies per Oompa: %d\n", factory->candies_per_oompa);

    





    // free factory at the end
    free(factory);
}