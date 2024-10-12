#include <iostream>
#include <stdio.h>
#include <windows.h>


SYSTEMTIME stUTC, stLocal;

bool moveFileToFolder(const char* sourcePath, const char* destinationPath)
{
    if (MoveFile(sourcePath, destinationPath)) {
        std::cout << "File moved successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to move file. Error: " << GetLastError() << std::endl;
        return false;
    }
}

bool createFolderIfNotExists(const char* folderPath) 
{
    DWORD fileAttributes = GetFileAttributesA(folderPath);

    if (fileAttributes == INVALID_FILE_ATTRIBUTES)
    {
        if (CreateDirectoryA(folderPath, NULL))
        {
            printf("Folder created: \"%s\". \n", folderPath);
            return true;
        } else {
            printf("Error creating folder: \"%s\". \n", folderPath);
            std::cerr<< "Error code: " << GetLastError() << std::endl;
            return false;
        }
    } else if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    } else {
        std::cerr << "Error: Path exists but is not a directory: " << folderPath << std::endl;
        return false;
    }
}

const char* getFileCreationYearInfo(const char* filePath)
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
        FileTimeToSystemTime(&creationTime, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
        auto creationYearWORD = stLocal.wYear;
        const char* creationYear = std::to_string(creationYearWORD).c_str();
        printf("File \"%s\" has the year of creation --> %s", filePath, creationYear);
        CloseHandle(hFile);
        return creationYear;
    } else {
        std::cerr << "Error retrieving file time!" << std::endl;
    }

    CloseHandle(hFile);
    return NULL;
}

void sortFilesByYearOfCreationWindows(const char* folderPathSource, const char* folderPathDestination)
{
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    if ((hFind = FindFirstFile(folderPathSource, &FindFileData)) != INVALID_HANDLE_VALUE)
    {
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                continue; // Skip directories for now. TODO: handle directory

            auto fullFilePathSource = folderPathSource + "\\" + FindFileData.cFileName;
            auto yearOfFileCreation = getFileCreationYearInfo(fullFilePathSource);
            auto fullFolderPathDestination = folderPathDestination + "\\" + yearOfFileCreation;
            
            if (createFolderIfNotExists(fullFolderPathDestination))
            {
                moveFileToFolder(fullFilePathSource, fullFolderPathDestination);
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
    else {
        printf("Terminal failure: Unable to open folder \"%s\".\n", folderPathFrom);
    }
}

int main(int argc, char *argv[]) 
{
    sortFilesByYearOfCreationWindows("H:\\projects\\YearlyMediaSorter\\testdata", "H:\projects\YearlyMediaSorter\testdata\moveto");
    return 0;
}
