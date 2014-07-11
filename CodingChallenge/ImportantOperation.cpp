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

using namespace std;

ImportantOperation::ImportantOperation() {
}

ImportantOperation::ImportantOperation(const ImportantOperation& orig) {
}

ImportantOperation::~ImportantOperation() {
}

int ImportantOperation::DoSomethingImportant(int fileNumber, int fileSize,
        int readCycles) {

    stringstream nameBuilder;
    nameBuilder << "importantDataFile" << fileNumber << ".txt";

    string fileName = nameBuilder.str();

    cout << fileName << endl;

    fstream fileStream(fileName,
            ios_base::binary | ios_base::in | ios_base::out | ios_base::trunc);

    int writeSum = WriteFile(fileStream, fileSize);

    int readSum = ReadFile(fileStream);

    cout << "Wrote: " << writeSum << endl << "Read: " << readSum << endl;

    return readSum;


}

int ImportantOperation::AddToCheckSum(int checksum, char datapoint) {
    return (checksum + datapoint) % 256;
}

int ImportantOperation::WriteFile(ostream& fileStream, int fileSize) {

    /*
     * Not a good checksum. Again, just trying to make some noise
     */
    int checksum = 0;
    for (int i = 0; i < fileSize; i++) {
        char random = (char) (rand() % 128);
        fileStream << random;
        checksum = AddToCheckSum(checksum, random);
    }

    fileStream.flush();

    return checksum;
}

int ImportantOperation::ReadFile(istream& fileStream) {

    fileStream.sync();
    fileStream.seekg(0);

    char input;
    int actualSize = 0;
    int checksum = 0;

    while (!fileStream.eof()) {
        input = fileStream.get();
        if (input != fstream::traits_type::eof()) {
            actualSize++;
            checksum = AddToCheckSum(checksum, input);
        }
    }

    return checksum;
}