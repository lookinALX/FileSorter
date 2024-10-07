#include <iostream>
#include <windows.h>

string GetFileInfo(string filePath){
    HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
}

int main(int argc, char *argv[]) {
    GetFileInfo("1234");
}

