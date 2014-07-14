/* 
 * File:   Result.h
 * Author: eah
 *
 * Created on July 13, 2014, 6:53 PM
 */

#ifndef RESULT_H
#define	RESULT_H

#include <string>
#include <fstream>
#include <chrono>

/*
 * Simple struct used to store, output, and summarize the results of 
 * our "Important operations".
 */
struct Result {
    Result();

    std::string label;
    int checkSum;
    std::fstream::traits_type::pos_type fileSize;
    int readCycles;
    int readFailures;
    std::chrono::milliseconds runTime;

};

std::ostream& operator<<(std::ostream& os, const Result& obj);
Result& operator+=(Result& accumulator, const Result& single);


#endif	/* RESULT_H */

