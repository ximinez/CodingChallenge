/* 
 * File:   ImportantOperation.cpp
 * Author: eah
 * 
 * Created on July 10, 2014, 9:16 PM
 */

#include "ImportantOperation.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

ImportantOperation::Result::Result()
: checkSum(0), fileSize(-1), readCycles(0), readFailures(0)
{
}

ostream& ImportantOperation::Result::dump(ostream& out)
{
    out << "Result for " << fileName
            << ": run time, " << runTime.count() << "ms"
            << ". checksum, " << checkSum
            << ". file size, " << fileSize
            << ". read cycles, " << readCycles
            << ". read failures, " << readFailures
            << endl;

    return out;
}

ImportantOperation::ImportantOperation(int _fileNumber, int _fileSize,
        int _readCycles)
: fileSize(_fileSize), readCycles(_readCycles)
{

    stringstream nameBuilder;
    nameBuilder << "importantDataFile" << _fileNumber << ".txt";

    fileName = nameBuilder.str();

    //    cout << fileName << endl;

}

ImportantOperation::ImportantOperation(const ImportantOperation& orig)
: fileName(orig.fileName), fileSize(orig.fileSize),
readCycles(orig.readCycles)
{
}

ImportantOperation::~ImportantOperation()
{
    // Delete the file if it was created.
    ifstream fileExists(fileName);
    if (fileExists.rdstate() == ios_base::goodbit)
    {
        fileExists.close();
        remove(fileName.c_str());
    }
}

ImportantOperation::Result ImportantOperation::DoSomethingImportant()
{
    auto startTime = chrono::steady_clock::now();
    Result result;

    result.fileName = fileName;

    fstream fileStream(fileName,
            ios_base::binary | ios_base::in | ios_base::out | ios_base::trunc);

    WriteFile(fileStream, fileSize, &result);

    for (int i = 0; i < readCycles; i++)
    {
        ReadFile(fileStream, &result);
    }

    auto runTime = chrono::steady_clock::now() - startTime;
    result.runTime = chrono::duration_cast<chrono::milliseconds > (runTime);

    return result;


}

void ImportantOperation::DoSomethingWithAPromise(promise<ImportantOperation::Result>& resultPromise)
{
    try
    {
        resultPromise.set_value(DoSomethingImportant());
    }
    catch (...)
    {
        resultPromise.set_exception(current_exception());
    }
}

future<ImportantOperation::Result> ImportantOperation::DoSomethingAsyncronously()
{
    // future from a promise
    promise<ImportantOperation::Result> resultPromise;
    future<ImportantOperation::Result> resultFuture = resultPromise.get_future();

    thread t(
            [](ImportantOperation* op, std::promise<ImportantOperation::Result> p)
    {
        op->DoSomethingWithAPromise(p);
    },
    this, std::move(resultPromise)
            );

    t.detach();

    return resultFuture;
}

int ImportantOperation::AddToCheckSum(int checksum, char datapoint)
{
    return (checksum + datapoint) % 256;
}

int ImportantOperation::WriteFile(ostream& fileStream, int fileSize, Result* pResult)
{

    /*
     * Not a good checksum. Again, just trying to make some noise
     */
    int checksum = 0;
    for (int i = 0; i < fileSize; i++)
    {
        char random = (char) (rand() % 128);
        fileStream << random;
        checksum = AddToCheckSum(checksum, random);
    }

    auto actualSize = fileStream.tellp();
    //    cout << "Wrote: " << checksum << ". " << actualSize << endl;
    if (pResult)
    {
        pResult->checkSum = checksum;
        pResult->fileSize = actualSize;
    }

    fileStream.flush();

    return checksum;
}

int ImportantOperation::ReadFile(istream& fileStream, Result* pResult)
{

    fileStream.clear();
    fileStream.sync();
    fileStream.seekg(0);

    char input;
    int actualSize = 0;
    int checksum = 0;

    auto state = fileStream.rdstate();

    while (fileStream.good())
    {
        input = fileStream.get();
        if (input != fstream::traits_type::eof())
        {
            actualSize++;
            checksum = AddToCheckSum(checksum, input);
        }
    }

    if (pResult)
    {
        //        cout << "Read: " << checksum << ", " << pResult->checkSum
        //                << ". " << actualSize << ", " << pResult->fileSize
        //                << endl;
        if (checksum != pResult->checkSum
                || actualSize != pResult->fileSize)
            pResult->readFailures++;
        pResult->readCycles++;
    }
    //    else
    //    {
    //        cout << "Read: " << checksum << endl;
    //    }

    return checksum;
}