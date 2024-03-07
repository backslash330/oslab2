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

    // comfirm creation
    // printf("Oompa Loompa created\n");
    // print the memory address for this oompa loompa

    // print the memory address for this oompa loompa
    pthread_t rawid = pthread_self();
    //printf("Oompa Loompa memory address: %lu\n", (unsigned long)rawid);

//     // Convert thread id to string
    char tid_str[20];
    sprintf(tid_str, "%lu", (unsigned long)rawid);
//     //printf("Oompa Loompa thread id: %s\n", tid_str);

//     // create color string (color + tid)
//     char *color = malloc(strlen("color") + strlen(tid_str) + 1);
//     strcpy(color, "color");
//     strcat(color, tid_str);
//    // printf("Oompa Loompa color: %s\n", color);

    // per email, we need to randomly pick a number from the color_names array
    int color_index = rand() % 10;
    char *color = malloc(strlen(factory->color_names[color_index]) + 1);
    strcpy(color, factory->color_names[color_index]);
    strcat(color, tid_str);
    // printf("Oompa Loompa color: %s\n", color);

    // Use the producer slide to run the critical section
    int i;
    // we need a string which is " - " + i to use later
    char *current_candy = malloc(strlen(color) + strlen(" - ") + 2);
    char *suffix = malloc(strlen(" - ") + 2);
    for(i = 0; i < factory->candies_per_oompa_max; i++){


        // The suffix needs to be " - " + i
        sprintf(suffix, " - %d", i);
        //printf("Suffix: %s\n", suffix);


        // Allocate new memory for each candy
        current_candy = malloc(strlen(color) + strlen(suffix) + 1);
        strcpy(current_candy, color);
        strcat(current_candy, suffix);
        // printf("Oompa Loompa %s created candy (%s)\n", tid_str, current_candy);

        
        pthread_mutex_lock(&the_mutex);
        while (factory->assembly_line_index == factory->assembly_line_max) {
           // printf("Child %s is waiting for a candy to take from the assembly line\n", tid_str);
                       if (factory->completed_children == factory->children_max) {
                pthread_mutex_unlock(&the_mutex);
                pthread_cond_broadcast(&condp);
                pthread_exit(NULL);
                return NULL;
            }
            pthread_cond_wait(&condp, &the_mutex);
            // printf("Oompa Loompa %s is done waiting for a candy to take from the assembly line\n", tid_str);
            // If the assembly is full and the children are done, break the loop
            // printf("completed children: %d\n", factory->completed_children);
            // printf("children max: %d\n", factory->children_max);
            if (factory->completed_children == factory->children_max) {
                pthread_mutex_unlock(&the_mutex);
                pthread_cond_broadcast(&condp);
                pthread_exit(NULL);
                return NULL;
            }
        }

        // This is the critical section !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        // put the candy on the assembly line in the factory 
        // Add the value of current_candy to the assembly line, NOT the pointer
        // since assembly line is a char **, it takes pointer to a char
        // However, current_candy will be freed after the critical section, so we need to copy the value of current_candy to the assembly line
        factory->assembly_line[factory->assembly_line_index] = malloc(strlen(current_candy) + 1);
        strcpy(factory->assembly_line[factory->assembly_line_index], current_candy);
    

        // printf("Oompa Loompa %s added candy (%s) to assembly line at index %d\n", tid_str, current_candy, factory->assembly_line_index);
        factory->assembly_line_index++;
                // debug: check the assembly line
        // for (int j = 0; j < factory->assembly_line_max; j++) {
        //     printf("Assembly line from OL %d: %s\n", j, factory->assembly_line[j]);
        // }

        // signal the consumer
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&the_mutex);
        // Critical section ends here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // clear the current candy
        strcpy(current_candy, "");

    }


    //There is a deadlock condition. If a child goes to sleep and the production is done, the child will never wake up.
    // We therefore need to wake all the children up

    // free the memory
    free(color);
    free(current_candy);
    free(suffix);

    printf("Oompa Loompa %lu done\n", (unsigned long)rawid);
    pthread_exit(NULL);
    return NULL;
}

