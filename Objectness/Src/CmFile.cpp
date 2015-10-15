#include "kyheader.h"


#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <Commdlg.h>
#include <ShellAPI.h>
#else
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#endif


// Get image names from a wildcard. Eg: GetNames("D:\\*.jpg", imgNames);
int CmFile::GetNames(CStr &nameW, vecS &names, string &dir)
{
    dir = GetFolder(nameW);
    names.clear();
    names.reserve(6000);
    WIN32_FIND_DATAA fileFindData;
    HANDLE hFind = ::FindFirstFileA(_S(nameW), &fileFindData);
    if (hFind == INVALID_HANDLE_VALUE)
        return 0;

    do{
        if (fileFindData.cFileName[0] == '.')
            continue; // filter the '..' and '.' in the path
        if (fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue; // Ignore sub-folders
        names.push_back(fileFindData.cFileName);
    } while (::FindNextFileA(hFind, &fileFindData));
    FindClose(hFind);
    return (int)names.size();
}
int CmFile::GetSubFolders(CStr& folder, vecS& subFolders)
{
    subFolders.clear();
    WIN32_FIND_DATAA fileFindData;
    string nameWC = folder + "\\*";
    HANDLE hFind = ::FindFirstFileA(nameWC.c_str(), &fileFindData);
    if (hFind == INVALID_HANDLE_VALUE)
        return 0;

    do {
        if (fileFindData.cFileName[0] == '.')
            continue; // filter the '..' and '.' in the path
        if (fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            subFolders.push_back(fileFindData.cFileName);
    } while (::FindNextFileA(hFind, &fileFindData));
    FindClose(hFind);
    return (int)subFolders.size();
}

int CmFile::GetNames(CStr& rootFolder, CStr &fileW, vecS &names)
{
    string tmp;
    GetNames(rootFolder + fileW, names, tmp);
    vecS subFolders, tmpNames;
    int subNum = CmFile::GetSubFolders(rootFolder, subFolders);
    for (int i = 0; i < subNum; i++){
        subFolders[i] += "/";
        int subNum = GetNames(rootFolder + subFolders[i], fileW, tmpNames);
        for (int j = 0; j < subNum; j++)
            names.push_back(subFolders[i] + tmpNames[j]);
    }
    return (int)names.size();
}
int CmFile::GetNamesNE(CStr& nameWC, vecS &names)
{
    string dir = string();
    string ext = string();
    int fNum = GetNames(nameWC, names, dir);
    ext = GetExtention(nameWC);
    for (int i = 0; i < fNum; i++)
        names[i] = GetNameNE(names[i]);
    return fNum;
}
int CmFile::GetNamesNE(CStr& nameWC, vecS &names, string &dir, string &ext)
{
    int fNum = GetNames(nameWC, names, dir);
    ext = GetExtention(nameWC);
    for (int i = 0; i < fNum; i++)
        names[i] = GetNameNE(names[i]);
    return fNum;
}
int CmFile::GetNamesNE(CStr& rootFolder, CStr &fileW, vecS &names)
{
    int fNum = GetNames(rootFolder, fileW, names);
    int extS = GetExtention(fileW).size();
    for (int i = 0; i < fNum; i++)
        names[i].resize(names[i].size() - extS);
    return fNum;
}
bool CmFile::MkDir(CStr &_path)
{
    if(_path.size() == 0)
        return false;
    static char buffer[1024];
    strcpy(buffer, _S(_path));
#ifdef _WIN32
    for (int i = 0; buffer[i] != 0; i ++) {
        if (buffer[i] == '\\' || buffer[i] == '/') {
            buffer[i] = '\0';
            CreateDirectoryA(buffer, 0);
            buffer[i] = '/';
        }
    }
    return CreateDirectoryA(_S(_path), 0);
#else
    for (int i = 0; buffer[i] != 0; i ++) {
        if (buffer[i] == '\\' || buffer[i] == '/') {
            buffer[i] = '\0';
            mkdir(buffer, 0755);
            buffer[i] = '/';
        }
    }
    return mkdir(_S(_path), 0755);
#endif
}
vecS CmFile::loadStrList(CStr &fName)
{
    ifstream fIn(fName);
    string line;
    vecS strs;
    while(getline(fIn, line) && line.size()){
        //unsigned sz = line.size();
        //line.resize(sz - 1); //Please use script to convert the VOC format data into the OpenCV format data
        //line.resize(sz);
        strs.push_back(line);
    }
    return strs;
}
bool CmFile::writeStrList(CStr &fName, const vecS &strs)
{
    FILE *f = fopen(_S(fName), "w");
    if (f == NULL)
        return false;
    for (size_t i = 0; i < strs.size(); i++)
        fprintf(f, "%s\n", _S(strs[i]));
    fclose(f);
    return true;
}
