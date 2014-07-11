/* 
 * File:   main.cpp
 * Author: eah
 *
 * Created on July 10, 2014, 9:06 PM
 */

#include <cstdlib>
#include <thread>

#include "ImportantOperation.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    /* Seed the random number generator, here, but
     * we're not making any effort to generate strong random numbers.
     * The point here is to make some noise, not to have it
     * mean anything or be useful for anything.
     */
    srand(time(0));

    ImportantOperation op;
    op.DoSomethingImportant(1, 100, 1);
    
    return 0;
}

