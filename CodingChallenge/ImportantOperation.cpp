/* 
 * File:   ImportantOperation.cpp
 * Author: eah
 * 
 * Created on July 10, 2014, 9:16 PM
 */

#include "ImportantOperation.h"

#include <sstream>
#include <iostream>
#include <mutex>

using namespace std;

/*
 * The fileNumber / fileName doesn't really matter to the functioning of the 
 * program. I just want to make sure each ImportantOperation uses a 
 * unique name.
 */
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
: fileName(orig.fileName), fileSize(orig.fileSize), readCycles(orig.readCycles)
{
}

ImportantOperation::~ImportantOperation()
{
    /*
     * Delete the file if it was left hanging around.
     * eg. Through an unhandled exception, premature application exit, etc.
     */
    ifstream fileExists(fileName);
    if (fileExists.rdstate() == ios_base::goodbit)
    {
        fileExists.close();
        remove(fileName.c_str());
    }
}

/*
 * The important thing here isn't really, er, important, as long as it's doing
 * something that takes some measurable time, and can be parallelized
 * 
 * In this case, I take advantage of the natural blocking that comes with I/O
 * to write [fileSize] random characters to a [fileName], compute a trivial
 * checksum, then read them back [readCycles] times, verifying that it gets
 * the same checksum and number of bytes.
 */
Result ImportantOperation::DoSomethingImportant()
{
    auto startTime = chrono::steady_clock::now();
    Result result;

    result.label = fileName;

    fstream fileStream(fileName,
            ios_base::binary | ios_base::in | ios_base::out | ios_base::trunc);

    WriteFile(fileStream, fileSize, &result);

    for (int i = 0; i < readCycles; i++)
    {
        ReadFile(fileStream, &result);
    }

    auto runTime = chrono::steady_clock::now() - startTime;
    result.runTime = chrono::duration_cast<chrono::milliseconds > (runTime);

    fileStream.close();
    remove(fileName.c_str());


    return result;
}

/*
 * Create an independent thread that calls DoSomethingImportant. 
 * Use a promise/future to get the Result back to the caller when
 * the thread finishes.
 * 
 * Uses the DoSomethingWithAPromise just to keep the code a little 
 * more readable.
 */
future<Result> ImportantOperation::DoSomethingAsyncronously()
{
    // future from a promise
    promise<Result> resultPromise;
    future<Result> resultFuture = resultPromise.get_future();

    thread t(
            [](ImportantOperation* op, std::promise<Result> p)
    {
        op->DoSomethingWithAPromise(p);
    },
    this, std::move(resultPromise)
            );

    /*
     * detach the thread from being owned by this, or any object.
     * This will allow the thread to continue running in the background
     * when t goes out of scope and is destructed.
     */
    t.detach();

    return resultFuture;
}

/*
 * This could have been included in the lambda used in DoSomethingAsyncronously,
 * but thats ugly.
 */
void ImportantOperation::DoSomethingWithAPromise(promise<Result>& resultPromise)
{
    try
    {
        resultPromise.set_value(DoSomethingImportant());
    }
    catch (...)
    {
        // A little bit of exception handling
        resultPromise.set_exception(current_exception());
    }
}

/*
 * Not a good checksum. Just trying to make some noise
 */
int ImportantOperation::AddToCheckSum(int checksum, char datapoint)
{
    return (checksum + datapoint) % 256;
}

/*
 * Write [fileSize] random bytes to the stream, and update the Result if any.
 */
int ImportantOperation::WriteFile(ostream& fileStream, int fileSize, Result* pResult)
{

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

/*
 * Read all the bytes in the stream and check if they match the Result.
 */
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