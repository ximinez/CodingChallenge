/* 
 * File:   main.cpp
 * Author: eah
 *
 * Created on July 10, 2014, 9:06 PM
 */

#include <cstdlib>
#include <thread>
#include <future>
#include <iostream>
#include <vector>

#include "ImportantOperation.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{

    /* Seed the random number generator, here, but
     * we're not making any effort to generate strong random numbers.
     * The point here is to make some noise, not to have it
     * mean anything or be useful for anything.
     */
    srand(time(0));

    // create all the operation objects
    const int count = 100;
    vector < ImportantOperation > operations;
    for (int i = 0; i < count; i++)
    {
        ImportantOperation op(i, 1000000, 100);
        operations.push_back(op);
    }

    // do all the operations serially
    auto startTime = chrono::steady_clock::now();
    for (auto& op : operations)
    {
        auto result = op.DoSomethingImportant();

        result.dump(cout);
    }
    auto runTime = chrono::duration_cast<chrono::milliseconds > (chrono::steady_clock::now() - startTime);

    cout << "Serial run time, " << runTime.count() << "ms" << endl;

    startTime = chrono::steady_clock::now();
    vector< future<ImportantOperation::Result > > resultFutures;
    for (auto& op : operations)
    {
        future<ImportantOperation::Result> opFuture = op.DoSomethingAsyncronously();

        resultFutures.push_back(move(opFuture));
    }
    for(auto & resultFuture : resultFutures)
    {
        auto result = resultFuture.get();
        result.dump(cout);
    }
    runTime = chrono::duration_cast<chrono::milliseconds > (chrono::steady_clock::now() - startTime);

    cout << "Parallel run time, " << runTime.count() << "ms" << endl;

    return 0;
}

