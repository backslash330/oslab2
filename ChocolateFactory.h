// The Chocolate Factory:
// Wonka's factory contains two types of worker threads—oompa loompas and children. Each oompa
// loompa will produce a single color of candy; each child will create a box of candies of assorted
// colors. Oompa loompas produce one candy at a time and then place the candy (when there is
// room) onto the assembly line (a limited-sized buffer). Children remove one candy at a time from
// the assembly line until they have enough candies to fill a box. Once a box of candies has been
// filled, the child will alert Wonka and begin filling a new box.
// Wonka requires that the following controls for his factory be installed in his glass elevator:
// § o: the number of oompa loompa threads
// § c: the number of children threads
// § a: the size of the assembly line
// § n: the number of candies per box
// § t: the number of candies each oompa loompa will produce
// Your program must accept these five inputs from the command line. You can assume that the
// inputs are always valid integers in the range [1 : 231-1], and are always provided in the following
// order: o, c, a, n, t.
// i.e. $> ./ChocolateFactory 5 5 10 100 1000

// Library includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int oompa_loompas_max;
    int children_max;
    int assembly_line_max;
    int candies_per_box_max;
    int candies_per_oompa_max;
    // we need to add the assembly line here as pthread only accepts one argument
    char **assembly_line;
    int assembly_line_index;
    // this is for the case that their are less candies than the candies per box
    int done_production;
} ChocolateFactory;