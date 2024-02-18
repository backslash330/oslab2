// Oompa Loompas (oompa-loompas.c)
// Each oompa loompa produces a candy of unique color and each candy is numbered sequentially.
// For example, oompa loompa 1 might produce candies: AliceBlue 1, AliceBlue 2, AliceBlue 3, … and
// oompa loompa 2 candies: PeachPuff 1, PeachPuff 2, PeachPuff 3, …, and so forth. You can assign
// any color you wish to each oompa Loompa but the names must be unique (so you can tell which
// oompa loompa produced which candy). After producing a candy, the oompa loompa will place
// it onto the assembly line (the bounded-buffer array). If the assembly line is full, the oompa loompa
// must wait until a slot becomes available.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

// Oompa Loompas (oompa-loompas.c)
// ...

void *oompa_loompa_worker(void *factory_ptr){
    // declare the factory
    ChocolateFactory *factory = (ChocolateFactory *)factory_ptr;

    // extern the global variables
    extern pthread_mutex_t the_mutex;
    extern pthread_cond_t condc, condp;
    extern int buffer;

    // comfirm creation
    // printf("Oompa Loompa created\n");
    // print the memory address for this oompa loompa

    // print the memory address for this oompa loompa
    pthread_t rawid = pthread_self();
    //printf("Oompa Loompa memory address: %lu\n", (unsigned long)rawid);

    // Convert thread id to string
    char tid_str[20];
    sprintf(tid_str, "%lu", (unsigned long)rawid);
    //printf("Oompa Loompa thread id: %s\n", tid_str);

    // create color string (color + tid)
    char *color = malloc(strlen("color") + strlen(tid_str) + 1);
    strcpy(color, "color");
    strcat(color, tid_str);
   // printf("Oompa Loompa color: %s\n", color);

    // Use the producer slide to run the critical section
    int i;
    char *i_str = malloc(20);
    // we need a string which is " - " + i to use later
    char *current_candy = malloc(strlen(color) + strlen(" - ") + 2);
    char *suffix = malloc(strlen(" - ") + 2);
    for(i = 0; i < factory->candies_per_oompa_max; i++){
        pthread_mutex_lock(&the_mutex);
        while (buffer != 0 || factory->assembly_line_index == factory->assembly_line_max) {
            pthread_cond_wait(&condp, &the_mutex);
        }
        buffer = 1;

       // printf("buffer set to %d by Oompa Loompa %s\n", buffer, tid_str);
        // This is the critical section !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //suffix = " - ";
        // sprintf(i_str, "%d", i);
        // printf("i_str: %s\n", i_str);
        // printf("suffix: %s\n", suffix);
        strcpy(current_candy, color);
        strcat(current_candy, suffix);
        printf("Oompa Loompa %s created candy (%s)\n", tid_str, current_candy);
        // put the candy on the assembly line in the factory 
        factory->assembly_line[factory->assembly_line_index] = current_candy;
        printf("Oompa Loompa %s added candy (%s) to assembly line at index %d\n", tid_str, current_candy, factory->assembly_line_index);
        factory->assembly_line_index++;
                // debug: check the assembly line
        for (int j = 0; j < factory->assembly_line_max; j++) {
            printf("Assembly line from OL %d: %s\n", j, factory->assembly_line[j]);
        }
        // clear the current candy and the buffer
        strcpy(current_candy, "");
        // signal the consumer
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&the_mutex);
        // Critical section ends here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    // free the memory
    free(color);
    free(current_candy);
    free(suffix);
    free(i_str);

    pthread_exit(NULL);
    return NULL;
}
// void *oompa_loompa_worker(void *factory){
//     // comfirm creation
//     // printf("Oompa Loompa created\n");

//     // print the memory address for this oompa loompa
//     long unsigned rawid = pthread_self();
//     int tid = (int)rawid;
//     printf("Oompa Loompa memory address: %lu\n", rawid);
//     char tid_str = (char)tid;
//     printf("Oompa Loompa thread id: %d\n", tid_str);
    
//     //printf("Oompa Loompa thread id: %d\n", tid);

//     // create color string (color + tid)
//     char *color = malloc(strlen("color") + strlen(&tid_str) + 1);
//     strcpy(color, "color");
//     strcat(color, &tid_str);
//     printf("Oompa Loompa color: %s\n", color);


//     // Use the producer slide to run the critical section
    

//     pthread_exit(NULL);
//     return NULL;
// }
