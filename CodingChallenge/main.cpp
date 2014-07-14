/* 
 * File:   main.cpp
 * Author: eah
 *
 * Created on July 10, 2014, 9:06 PM
 */

#include "ImportantOperation.h"

#include <cstdlib>
#include <thread>
#include <future>
#include <iostream>
#include <vector>

using namespace std;

/*
 * This program simulates an application doing some "Important operation".
 * The class that does this is called "Important operation", just to keep
 * things clear.
 * 
 * The operations are done in two passes. 
 * The first pass simply loops through the ImportantOperation objects,
 * and does the operation serially.
 * The second pass loops through the ImportantOperations twice, first to 
 * fire off a thread to do the operation, then to wait for the results of
 * each thread.
 * 
 * Each pass collects statistics on the pass. The summary of statistics 
 * for both is displayed before exiting.
 */
int main(int argc, char** argv)
{

    /*
     * Seed the random number generator here.
     * We're not making any effort to generate strong random numbers.
     * The point here is to make some noise, and have each
     * "Important operation" show some uniqueness, but not to have it
     * mean anything or be useful for anything.
     * 
     * Obviously, if this was a cryptographic algorithm, I'd use something
     * more appropriate.
     */
    srand(time(0));

    // create all the "Important operation" objects
    const int count = 100;
    vector < ImportantOperation > operations;
    for (int i = 0; i < count; i++)
    {
        int fileSize = (rand() % 1000000) + 500000;
        int readCycles = (rand() % 100) + 50;
        ImportantOperation op(i, fileSize, readCycles);
        operations.push_back(op);
    }

    Result serialStats;
    serialStats.label = "Total serial processing";
    Result parallelStats;
    parallelStats.label = "Total parallel processing";

    // First pass: do all the operations serially
    auto startTime = chrono::steady_clock::now();
    for (auto& op : operations)
    {
        auto result = op.DoSomethingImportant();

        cout << result << endl;
        serialStats += result;
    }
    auto serialRunTime = chrono::duration_cast<chrono::milliseconds > (chrono::steady_clock::now() - startTime);

    cout << "Serial run time, " << serialRunTime.count() << "ms" << endl;

    // Second pass: do all of the operations in separate parallel threads
    startTime = chrono::steady_clock::now();
    vector< future<Result > > resultFutures;
    for (auto& op : operations)
    {
        future<Result> opFuture = op.DoSomethingAsyncronously();

        resultFutures.push_back(move(opFuture));
    }
    for (auto& resultFuture : resultFutures)
    {
        // It doesn't really matter what order the threads finish in,
        // but we'll retrieve and process the results in the same sequential
        // order for simplicity.
        auto result = resultFuture.get();

        cout << result << endl;
        parallelStats += result;
    }
    auto parallelRunTime = chrono::duration_cast<chrono::milliseconds > (chrono::steady_clock::now() - startTime);

    cout << "Parallel run time, " << parallelRunTime.count() << "ms" << endl;

    // Output the summarized result statistics
    cout << endl << endl
            << "Summary:" << endl
            << serialStats << endl
            << "\tActual total time: " << serialRunTime.count() << "ms" << endl
            << parallelStats << endl
            << "\tActual total time: " << parallelRunTime.count() << "ms" << endl;

    return 0;
}

