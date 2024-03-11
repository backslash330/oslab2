


// Name your programs as indicated below. Your executable should be named “ChocolateFactory”.
// Do not submit programs with executables named a.out! 

// Charlie (charlie.c)
// Your main function will be responsible for the overall control of the factory: creating the assembly
// line (bounded-buffer array), spawning o oompa loompa, and c child workers (threads). Once all
// of the oompa loompa's have finished producing candies, you must wait for all the candies
// remaining in the assembly line to get packaged then let the children go home for the day.
// nalmeida@Desktop-Nick:~/os/lab2$ gcc charlie.c -o ChocolateFactory
// nalmeida@Desktop-Nick:~/os/lab2$ ./ChocolateFactory 1 2 3 4 5
//gcc charlie.c -o ChocolateFactory -pthread

// our test values need to be EVEN for the program to complete. Otherwise a child or oompa loompa will be stuck in the while loop
// ./ChocolateFactory 5 5 10 100 100

// Library includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "ChocolateFactory.h"
#include "oompa-loompas.c"
#include "children.c"

// global variables
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
// create a string buffer to hold a candy that can be any size 
int buffer = 0;

int main(int argc, char *argv[]) {
    // create any necessary variables
    int status = 0;

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
    factory->oompa_loompas_max = atoi(argv[1]);
    factory->children_max = atoi(argv[2]);
    factory->assembly_line_max = atoi(argv[3]);
    factory->candies_per_box_max = atoi(argv[4]);
    factory->candies_per_oompa_max = atoi(argv[5]);
    factory->done_production = 0;

    // print the factory settings
    // printf("Factory Settings:\n");
    // printf("Oompa Loompas: %d\n", factory->oompa_loompas_max);
    // printf("Children: %d\n", factory->children_max);
    // printf("Assembly Line: %d\n", factory->assembly_line_max);
    // printf("Candies per Box: %d\n", factory->candies_per_box_max);
    // printf("Candies per Oompa: %d\n", factory->candies_per_oompa_max);

    // Per email, set ten color names (factory is char *color_names[10];)
    char *colors[] = {"Red", "Orange", "Yellow", "Green", "Blue", "Indigo", "Violet", "White", "Black", "Grey"};
    for (int i = 0; i < 10; i++) {
        factory->color_names[i] = malloc(strlen(colors[i]) + 1);  // +1 for the null terminator
        strcpy(factory->color_names[i], colors[i]);
    }

    // create the assembly line (bounded-buffer array) of size assembly_line that holds strings
    char **assembly_line = malloc(factory->assembly_line_max * sizeof(char *));
    for (int i = 0; i < factory->assembly_line_max; i++) {
        assembly_line[i] = malloc(100 * sizeof(char));
    }

    // add the assembly line to the factory
    factory->assembly_line = assembly_line;
    factory->assembly_line_index = 0;

    // create the semaphores using pthread
    pthread_mutex_init(&the_mutex, 0);  
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);

    // create the oompa loompas using pthread and oompa-loompa.c
    pthread_t oompa_loompa_threads[factory->oompa_loompas_max];
    for(int i = 0; i < factory->oompa_loompas_max; i++) {
        // printf("Creating oompa loompa %d...\n", i);
        status = pthread_create(&oompa_loompa_threads[i], NULL, oompa_loompa_worker, (void *)factory);
        if (status != 0) {
            printf("Oops.oompa loompa pthread_create returned error code %d\n", status);
            exit(EXIT_FAILURE);
        }
    }
    
    // create the children using pthread and children.c
    pthread_t children_threads[factory->children_max];
    for(int i = 0; i < factory->children_max; i++) {
        // printf("Creating child %d...\n", i);
        status = pthread_create(&children_threads[i], NULL, child_worker, (void *)factory);
        if (status != 0) {
            printf("Oops.children pthread_create returned error code %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    // wait for all the oompa loompas to finish producing candies
    for(int i = 0; i < factory->oompa_loompas_max; i++) {
        pthread_join(oompa_loompa_threads[i], NULL);
    }

    // printf("All oompa loompas compelte.\n");
    // set factory to done production
    factory->done_production = 1;
    // wake all the children up
    pthread_cond_broadcast(&condc);

    // wait for all the candies remaining in the assembly line to get packaged
    for(int i = 0; i < factory->children_max; i++) {
        pthread_join(children_threads[i], NULL);
    }

    // printf("All threads compelte.\n");

    // free memory at the end
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
    free(assembly_line);
    free(factory);

    // exit the program
    // printf("Ending program correctly.\n");
    return 0;
}