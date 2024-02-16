// Children (children.c)
// Each child will extract candies one at a time from the assembly line (the bounded-buffer array).
// After retrieving n candies, the child should shout (print to stdout) the contents of the candy box:
// Wonka, I have a box of candies containing: Beige 2, Bisque 3, Black 9, … Bisque 10

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ChocolateFactory.h>