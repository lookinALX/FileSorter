#include <iostream>
#include <stdio.h>
#include <windows.h>


void PrintFileTime(const FILETIME& ft) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    std::cout << stLocal.wYear << std::endl;
}


std::string GetFileInfo(const char* filePath)
{
    HANDLE hFile; 
    hFile = CreateFile(filePath,                     // name of the write
                    GENERIC_WRITE,                   // open for write
                    FILE_SHARE_WRITE,                // share write
                    NULL,                            // default security
                    OPEN_EXISTING,                   // create new file only
                    FILE_ATTRIBUTE_NORMAL,           // normal file
                    NULL);                           // no attr. template
    
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        printf("Terminal failure: Unable to open file \"%s\".\n", filePath);
        return NULL;
    }   

    FILETIME creationTime, accessTime, writeTime;
    if (GetFileTime(hFile, &creationTime, &accessTime, &writeTime)) {
        std::cout << "Creation Time: ";
        PrintFileTime(creationTime);
    } else {
        std::cerr << "Error retrieving file time!" << std::endl;
    }

    // Close the file handle
    CloseHandle(hFile);
    return "";
}

int main(int argc, char *argv[]) 
{
    GetFileInfo("H:\\projects\\YearlyMediaSorter\\testdata\\filetomove.jpg");
    return 0;
}

