// Children (children.c)
// Each child will extract candies one at a time from the assembly line (the bounded-buffer array).
// After retrieving n candies, the child should shout (print to stdout) the contents of the candy box:
// Wonka, I have a box of candies containing: Beige 2, Bisque 3, Black 9, … Bisque 10

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


void *child_worker(void *factory_ptr){
    // this should be similar to the consumer slide

    // declare the factory
    ChocolateFactory *factory = (ChocolateFactory *)factory_ptr;
    int break_flag = 0;
    //extern the global variables
    extern pthread_mutex_t the_mutex;
    extern pthread_cond_t condc, condp;

    // print the memory address for this oompa loompa
    pthread_t rawid = pthread_self();
    //printf("Oompa Loompa memory address: %lu\n", (unsigned long)rawid);

    // Convert thread id to string
    char tid_str[20];
    sprintf(tid_str, "%lu", (unsigned long)rawid);
    //printf("Oompa Loompa thread id: %s\n", tid_str);
    // create candy box
    char *candy_box[factory->candies_per_box_max];
    int candy_box_index = 0;

    // crit loop
    int i;
    for(i = 0; i < factory->candies_per_box_max; i++){
        // if production is done, break the loop
        // if (factory->done_production == 1 && factory->assembly_line_index < factory->candies_per_box_max-1) {
        //     printf("Child %s breaking loop...1\n", tid_str);
        //     break_flag = 1;
        //     pthread_exit(NULL);
        //     return NULL;
        // }
        pthread_mutex_lock(&the_mutex);
        while (factory->assembly_line_index == 0) {
            // printf("Child %s is waiting for a candy to take from the assembly line\n", tid_str);
            // print current while loop conditions
            // printf("assembly_line_index: %d\n", factory->assembly_line_index);
            //             if (factory->done_production == 1 && factory->assembly_line_index < factory->candies_per_box_max-1) {
            //     printf("Child %s breaking loop...2\n", tid_str);
            //     break_flag = 1;
            //     pthread_exit(NULL);
            //     return NULL;
            // }
            pthread_cond_wait(&condc, &the_mutex);
            // printf("Child %s is done waiting for a candy to take from the assembly line\n", tid_str);
            if (factory->done_production == 1 && factory->assembly_line_index < factory->candies_per_box_max-1) {
                printf("Child %s breaking loop...3\n", tid_str);
                break_flag = 1;
                pthread_exit(NULL);
                return NULL;
            }
        }

        // This is the critical section !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // take the candy from the assembly line in the factory
        // find the candy in the assembly line
        
        // debug: check the assembly line
        // for (int j = 0; j < factory->assembly_line_max; j++) {
        //     printf("Assembly line from child%s %d: %s\n",tid_str, j, factory->assembly_line[j]);
        // }
        char *candy = factory->assembly_line[(factory->assembly_line_index)-1];
        if (candy_box_index+1 == factory->candies_per_box_max) {
            factory->completed_children++;
        }
        // remove the candy from the assembly line
        factory->assembly_line[factory->assembly_line_index] = "";
        factory->assembly_line_index--;
        // signal the producer
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);
        // Critical section ends here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // printf("Child %s took candy (%s) from assembly line at index %d\n", tid_str, candy, factory->assembly_line_index);
        // debug: check the assembly line
        // for (int j = 0; j < factory->assembly_line_max; j++) {
        //     printf("Assembly line from child %d: %s\n", j, factory->assembly_line[j]);
        // }
        // add the candy to the box
        candy_box[candy_box_index] = candy;
        candy_box_index++;
        // if the box is full, print the box
        if(candy_box_index == factory->candies_per_box_max){
            printf("Child %s is shouting\n", tid_str);
            printf("Wonka, I have a box of candies containing: ");
            for(int j = 0; j < factory->candies_per_box_max; j++){
                printf("%s", candy_box[j]);
                if(j < factory->candies_per_box_max - 1){
                    printf(", ");
                }
            }
            printf("\n");
            candy_box_index = 0;
            // debug: check the assembly line
            // for (int j = 0; j < factory->assembly_line_max; j++) {
            //     printf("Assembly line from child %d: %s\n", j, factory->assembly_line[j]);
            // }
        }

        // printf("Child %d added candy to box\n", i);
    }


    if (break_flag == 1 ) {
        printf("Child %s is could not fill all boxes\n", tid_str);
    }

    printf("Child %s is done\n", tid_str);
    pthread_exit(NULL);
    return NULL;
}