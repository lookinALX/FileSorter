#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <shellapi.h>


SYSTEMTIME stUTC, stLocal;

void ensureFileIsWritable(const std::string& filePath)
{
    DWORD fileAttributes = GetFileAttributesA(filePath.c_str());
    if (fileAttributes & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN)) {
        fileAttributes &= ~(FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
        fileAttributes |= FILE_ATTRIBUTE_NORMAL; // Ensure normal attribute
        SetFileAttributesA(filePath.c_str(), fileAttributes);
    }
}

bool moveFileWithShell(const std::string& sourcePath, const std::string& destinationPath)
{
    SHFILEOPSTRUCT fileOp = { 0 };
    fileOp.wFunc = FO_MOVE;
    fileOp.pFrom = sourcePath.c_str();
    fileOp.pTo = destinationPath.c_str();
    fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

    if (SHFileOperation(&fileOp) == 0) {
        std::cout << "File moved successfully using Shell operation." << std::endl;
        std::cout << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to move file with Shell operation. Error: " << GetLastError() << std::endl;
        std::cout << std::endl;
        return false;
    }
}

bool moveFileToFolder(const std::string& sourcePath, const std::string& destinationPath)
{
    if (CopyFile(sourcePath.c_str(), destinationPath.c_str(), FALSE)) {
        if (DeleteFile(sourcePath.c_str())) {
            std::cout << "File moved successfully." << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to delete original file after copying. Error: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Failed to copy file. Error: " << GetLastError() << " Trying to move with Shell." << std::endl;
        moveFileWithShell(sourcePath, destinationPath);
    }
    return false;
}

bool createFolderIfNotExists(const std::string& folderPath) {
    size_t pos = 0;
    std::string path;
    while ((pos = folderPath.find('\\', pos)) != std::string::npos) {
        path = folderPath.substr(0, pos++);
        if (path.empty()) continue;
        if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
            if (!CreateDirectoryA(path.c_str(), NULL)) {
                std::cerr << "Failed to create directory: " << path << " Error: " << GetLastError() << std::endl;
                return false;
            }
        }
    }
    // Create the final folder in the path
    if (GetFileAttributesA(folderPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(folderPath.c_str(), NULL)) {
            std::cerr << "Failed to create directory: " << folderPath << " Error: " << GetLastError() << std::endl;
            return false;
        }
    }
    return true;
}

std::string getFileCreationYearInfo(const char* filePath)
{
    HANDLE hFile; 
    hFile = CreateFile(filePath,                     // name of the file
                    GENERIC_WRITE,                   // open for write
                    FILE_SHARE_WRITE,                // share write
                    NULL,                            // default security
                    OPEN_EXISTING,                   // create new file only
                    FILE_ATTRIBUTE_NORMAL,           // normal file
                    NULL);                           // no attr. template
    
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        printf("Terminal failure: Unable to open file \"%s\".\n", filePath);
        return "";
    }   

    FILETIME creationTime, accessTime, writeTime;
    if (GetFileTime(hFile, &creationTime, &accessTime, &writeTime)) {
        FileTimeToSystemTime(&creationTime, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
        auto creationYearWORD = stLocal.wYear;
        std::string creationYear = std::to_string(creationYearWORD);
        std::cout << "File " << std::string(filePath) << " has the year of creation--> " << creationYear << std::endl;
        CloseHandle(hFile);
        return creationYear;
    } else {
        std::cerr << "Error retrieving file time!" << std::endl;
    }

    CloseHandle(hFile);
    return "";
}

void sortFilesByYearOfCreationWindows(const char* folderPathSource, const char* folderPathDestination)
{
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    std::string searchPattern = std::string(folderPathSource) + "\\*";

    if ((hFind = FindFirstFile(searchPattern.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE)
    {
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Skip the "." and ".." directories
                if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0) {
                    std::cout << "Directory spotted: " << FindFileData.cFileName << "\n";
                }
                continue; // Skip directories
            }
            printf("\"%s\"\n", folderPathSource);
            auto fullFilePathSource = std::string(folderPathSource) + "\\" + FindFileData.cFileName;
            std::cout << "Processing file: \"" << fullFilePathSource << "\"\n";

            auto yearOfFileCreation = getFileCreationYearInfo(fullFilePathSource.c_str());
            auto fullFolderPathDestination = std::string(folderPathDestination) + "\\" + yearOfFileCreation;

            std::cout << "Destination folder: " << fullFolderPathDestination << "\"\n";
            
            if (createFolderIfNotExists(fullFolderPathDestination))
            {
                ensureFileIsWritable(fullFilePathSource);
                moveFileWithShell(fullFilePathSource, fullFolderPathDestination);
            }
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
    else {
        printf("Terminal failure: Unable to open folder \"%s\".\n", folderPathSource);
    }
}

int main(int argc, char *argv[]) 
{
    printf("Start\n");
    if (argc < 3) {
        std::string folderPathSource = "";
        std::string folderPathDestination = "";
        std::cout << "Source folder: " << "\n";
        std::cin >> folderPathSource;
        std::cout << "Destination folder: " << "\n";
        std::cin >> folderPathDestination;
        sortFilesByYearOfCreationWindows(folderPathSource.c_str(), folderPathDestination.c_str());
    }
    else {
        sortFilesByYearOfCreationWindows(argv[1], argv[2]);
    }
    return 0;
}

