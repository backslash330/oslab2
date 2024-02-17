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

void *oompa_loompa_worker(void *factory){
    // comfirm creation
    // printf("Oompa Loompa created\n");

    // print the memory address for this oompa loompa
    long unsigned rawid = pthread_self();
    int tid = (int)rawid;
    //printf("Oompa Loompa thread id: %d\n", tid);

    // create color string (color + tid)


    // Use the producer slide to run the critical section
    

    pthread_exit(NULL);
    return NULL;
}
