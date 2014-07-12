/* 
 * File:   ImportantOperation.h
 * Author: eah
 *
 * Created on July 10, 2014, 9:16 PM
 */

#ifndef IMPORTANTOPERATION_H
#define	IMPORTANTOPERATION_H

#include <fstream>
#include <chrono>
#include <future>

using namespace std;

class ImportantOperation {
public:

    struct Result {
        Result();
        ostream& dump(ostream& out);

        string fileName;
        int checkSum;
        fstream::traits_type::pos_type fileSize;
        int readCycles;
        int readFailures;
        chrono::milliseconds runTime;

    };
    ImportantOperation(int _fileNumber, int _fileSize, int _readCycles);
    ImportantOperation(const ImportantOperation& orig);
    virtual ~ImportantOperation();

    Result DoSomethingImportant();
    future<Result> DoSomethingAsyncronously();
private:
    int AddToCheckSum(int checksum, char datapoint);
    int WriteFile(ostream& fileStream, int fileSize, Result* pResult);
    int ReadFile(istream& fileStream, Result* pResult);
    void DoSomethingWithAPromise(promise<Result>& resultPromise);

    string fileName;
    int fileSize;
    int readCycles;
};

#endif	/* IMPORTANTOPERATION_H */

