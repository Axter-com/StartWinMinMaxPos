/*  
    Copyright ©2023 David Maisonave (www.Axter.com)
    GNU General Public License
    StartWinMinMaxPos is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
    This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    Usage:
    Use StartWinMinMaxPos to start up an executable minimize, maximize, and/or at a certain position and window size.
    To get it to work, copy StartWinMinMaxPos.exe to the same directory of the target executable.
    Rename the target executable by inserting ".Original" into the name.
    Examples: 
        "TheProgramName.Original.exe"
        "NotePad.Original.exe"
    Rename StartWinMinMaxPos.exe to the target executable.
    By default, the target program will start minimized. To get different behavior, create an ini file that has the same name as the executable but with ".ini" appended to the name.
    Example:
        "NotePad.exe.ini"
    See the StartWinMinMaxPos.exe.ini file for details on how to populate the INI file.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include <chrono>
#include <iomanip>
using namespace std;

wstring ExecutableIniFile;

wstring GetField(const wstring& FieldName, wstring defaultVal = L"")
{
    wchar_t ReturnBuff[512] = { 0 };
    GetPrivateProfileString(L"StartWinMinMaxPos", FieldName.c_str(), defaultVal.c_str(), ReturnBuff, sizeof(ReturnBuff), ExecutableIniFile.c_str());
    return ReturnBuff;
}

DWORD iGetField(const wstring& FieldName)
{
    wstring Value = GetField(FieldName);
    if (Value.size() == 0 || !isdigit(Value[0]))
        return 0;
    return stoi(Value);
}

wostream & Log()
{
    const wstring LogFileName = GetField(L"LogFile");
    if (LogFileName.empty())
        return wcout;
    static wofstream LogFile(LogFileName, ios::out | ios::app);
    if (LogFile.is_open())
        return LogFile;
    return wcout;
}

int PauseBeforeExit(int returnVal)
{
    DWORD PauseOnExit = iGetField(L"PauseOnExit");
    if (PauseOnExit)
        system("pause");
    return returnVal;
}

int wmain(int argc, wchar_t** argv)
{
    const wstring TargetSig = L"Original.";
    const wstring Executable = argv[0];
    const wstring Extension = Executable.substr(Executable.length() - 3);
    wstring ExecutableTarget = Executable.substr(0, Executable.length() - 3) + TargetSig + Extension;
    ExecutableIniFile = Executable + L".ini";
    const wstring DefaultIniFileName = L"StartWinMinMaxPos.exe.ini";
    if (GetFileAttributes(ExecutableIniFile.c_str()) == INVALID_FILE_ATTRIBUTES &&
        GetFileAttributes(DefaultIniFileName.c_str()) != INVALID_FILE_ATTRIBUTES)
        ExecutableIniFile = DefaultIniFileName;
    const wstring ArgPrefix = L" ";
    wstring PrgArg;
    for (int i = 1; i < argc; ++i) PrgArg += ArgPrefix + argv[i];
    PrgArg += GetField(L"ProgramArguments");
    ExecutableTarget = GetField(L"Executable", ExecutableTarget);

    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
#pragma warning(suppress : 4996)
    Log() << put_time(localtime(&now), L"%F %T") << endl;
    Log() << L"****************************" << endl;

    Log() << L"ExecutableTarget = " << ExecutableTarget << endl;
    Log() << L"PrgArg = " << PrgArg << endl;
    Log() << L"ExecutableIniFile = " << ExecutableIniFile << endl;


    STARTUPINFO si = { sizeof(si), 0 };
    wstring wShowWindow = GetField(L"ShowWindow", L"MIN");
    transform(wShowWindow.begin(), wShowWindow.end(), wShowWindow.begin(), ::toupper);
    if (wShowWindow == L"MIN" || wShowWindow == L"MINIMIZE" || wShowWindow == L"SW_MINIMIZE")
        si.wShowWindow = SW_MINIMIZE;
    else if (wShowWindow == L"MAX" || wShowWindow == L"MAXIMIZE" || wShowWindow == L"SW_MAXIMIZE")
        si.wShowWindow = SW_MAXIMIZE;
    else if (wShowWindow == L"NORM" || wShowWindow == L"SW_SHOWNORMAL" || wShowWindow == L"SW_NORMAL")
        si.wShowWindow = SW_NORMAL;
    else if (wShowWindow == L"SW_HIDE")
        si.wShowWindow = SW_HIDE;
    else if (wShowWindow == L"SW_SHOWMINIMIZED")
        si.wShowWindow = SW_SHOWMINIMIZED;
    else if (wShowWindow == L"SW_SHOWMAXIMIZED")
        si.wShowWindow = SW_SHOWMAXIMIZED;
    else if (wShowWindow == L"SW_SHOWNOACTIVATE")
        si.wShowWindow = SW_SHOWNOACTIVATE;
    else if (wShowWindow == L"SW_SHOW")
        si.wShowWindow = SW_SHOW;
    else if (wShowWindow == L"SW_SHOWMINNOACTIVE")
        si.wShowWindow = SW_SHOWMINNOACTIVE;
    else if (wShowWindow == L"SW_SHOWNA")
        si.wShowWindow = SW_SHOWNA;
    else if (wShowWindow == L"SW_RESTORE")
        si.wShowWindow = SW_RESTORE;
    else if (wShowWindow == L"SW_SHOWDEFAULT")
        si.wShowWindow = SW_SHOWDEFAULT;
    else if (wShowWindow == L"SW_FORCEMINIMIZE")
        si.wShowWindow = SW_FORCEMINIMIZE;
    else if (wShowWindow == L"SW_MAX")
        si.wShowWindow = SW_MAX;
    else
        si.wShowWindow = SW_MINIMIZE;
    Log() << L"wShowWindow = " << si.wShowWindow << endl;

    DWORD AddTodwFlags = iGetField(L"AddTodwFlags");
    Log() << L"AddTodwFlags = " << AddTodwFlags << endl;
    si.dwFlags = STARTF_USESHOWWINDOW | AddTodwFlags;
    si.dwX = iGetField(L"PosX");
    si.dwY = iGetField(L"PosY");
    if (si.dwX || si.dwY)
        si.dwFlags |= STARTF_USEPOSITION;
    si.dwXSize = iGetField(L"SizeX");
    si.dwYSize = iGetField(L"SizeY");
    if (si.dwXSize && si.dwYSize)
        si.dwFlags |= STARTF_USESIZE;
    si.dwXCountChars = iGetField(L"dwXCountChars");
    si.dwYCountChars = iGetField(L"dwYCountChars");
    if (si.dwXCountChars || si.dwYCountChars)
        si.dwFlags |= STARTF_USECOUNTCHARS;
    si.dwFillAttribute = iGetField(L"dwFillAttribute");
    if (si.dwFillAttribute)
        si.dwFlags |= STARTF_USEFILLATTRIBUTE;

    const wstring Title = GetField(L"Title");
    if (Title.size())
    {
        si.lpTitle = const_cast<LPWSTR>(Title.c_str());
        Log() << L"Title = " << si.lpTitle << endl;
    }
    else
    {
        const wstring LinkName = GetField(L"LinkName");
        if (LinkName.size())
        {
            si.lpTitle = const_cast<LPWSTR>(LinkName.c_str());
            si.dwFlags |= STARTF_TITLEISLINKNAME;
            Log() << L"LinkName = " << si.lpTitle << endl;
        }
        else
        {
            const wstring AppUserModelID = GetField(L"AppUserModelID");
            if (AppUserModelID.size())
            {
                si.lpTitle = const_cast<LPWSTR>(AppUserModelID.c_str());
                si.dwFlags |= STARTF_TITLEISAPPID;
                Log() << L"AppUserModelID = " << si.lpTitle << endl;
            }

        }
    }
    Log() << L"dwFlags = " << si.dwFlags << endl;

    PROCESS_INFORMATION pi = { 0 };
    if (!CreateProcess(
        const_cast<LPWSTR>(ExecutableTarget.c_str()),   // Application path
        const_cast<LPWSTR>(PrgArg.c_str()),             // Application arguments
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        Log() << L"CreateProcess failed" << GetLastError() << endl;
        return PauseBeforeExit(-1);
    }

    Log() << L"CreateProcess Success" << endl;
    return PauseBeforeExit(0);
}
