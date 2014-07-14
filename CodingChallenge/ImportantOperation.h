/* 
 * File:   ImportantOperation.h
 * Author: eah
 *
 * Created on July 10, 2014, 9:16 PM
 */

#ifndef IMPORTANTOPERATION_H
#define	IMPORTANTOPERATION_H

#include "Result.h"

#include <future>

using namespace std;

/*
 * Used to simulate some Important Operation that needs to be done
 * several times.
 * 
 * The meaningful functions are "DoSomethingImportant" 
 * and "DoSomethingAsyncronously". They both do the same thing,
 * but of course, "DoSomethingAsyncronously" does it in another
 * thread and returns a "future" that will hold the relevant 
 * Result when it is ready.
 */
class ImportantOperation {
public:

    ImportantOperation(int _fileNumber, int _fileSize, int _readCycles);
    ImportantOperation(const ImportantOperation& orig);
    virtual ~ImportantOperation();

    Result DoSomethingImportant();
    future<Result> DoSomethingAsyncronously();
private:
    static int AddToCheckSum(int checksum, char datapoint);
    static int WriteFile(ostream& fileStream, int fileSize, Result* pResult);
    static int ReadFile(istream& fileStream, Result* pResult);
    void DoSomethingWithAPromise(promise<Result>& resultPromise);

    string fileName;
    int fileSize;
    int readCycles;
};

#endif	/* IMPORTANTOPERATION_H */

