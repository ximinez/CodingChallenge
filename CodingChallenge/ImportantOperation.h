/* 
 * File:   ImportantOperation.h
 * Author: eah
 *
 * Created on July 10, 2014, 9:16 PM
 */

#ifndef IMPORTANTOPERATION_H
#define	IMPORTANTOPERATION_H

#include <fstream>

using namespace std;

class ImportantOperation {
public:
    ImportantOperation();
    ImportantOperation(const ImportantOperation& orig);
    virtual ~ImportantOperation();

    int DoSomethingImportant(int fileNumber, int fileSize, int readCycles);
private:
    int AddToCheckSum(int checksum, char datapoint);
    int WriteFile(ostream& fileStream, int fileSize);
    int ReadFile(istream& fileStream);
};

#endif	/* IMPORTANTOPERATION_H */

