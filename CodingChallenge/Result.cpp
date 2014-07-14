/* 
 * File:   Result.cpp
 * Author: eah
 * 
 * Created on July 13, 2014, 6:53 PM
 */

#include "Result.h"

using namespace std;

Result::Result()
: checkSum(0), fileSize(0), readCycles(0), readFailures(0), runTime(0)
{
}

ostream& operator<<(ostream& out, const Result& obj)
{
    out << "Result for " << obj.label
            << ": run time, " << obj.runTime.count() << "ms"
            << ". file size, " << obj.fileSize
            << ". read cycles, " << obj.readCycles
            << ". read failures, " << obj.readFailures;

    return out;
}

Result& operator+=(Result& accumulator, const Result& single)
{
    accumulator.runTime += single.runTime;
    accumulator.fileSize += single.fileSize;
    accumulator.readCycles += single.readCycles;
    accumulator.readFailures += single.readFailures;

    return accumulator;
}

