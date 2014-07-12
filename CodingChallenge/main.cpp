/* 
 * File:   main.cpp
 * Author: eah
 *
 * Created on July 10, 2014, 9:06 PM
 */

#include <cstdlib>
#include <thread>
#include <iostream>

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
    
    const int count = 100;
    unique_ptr<ImportantOperation> operations[count];
    

    ImportantOperation op(1, 1000000, 100);
    auto result = op.DoSomethingImportant();
    
    cout << "Result for " << result.fileName
            << ": run time, " << result.runTime.count() << "ms"
            << ". checksum, " << result.checkSum
            << ". file size, " << result.fileSize
            << ". read cycles, " << result.readCycles
            << ". read failures, " << result.readFailures
            << endl;
    
    return 0;
}

