// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

using namespace  std;
#include <iostream>
#include <Windows.h>
#include "atlbase.h"
#include "atlstr.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <ShlObj.h>
#include <windows.h>
#include <ShlObj.h>
#include <iostream>
#include <string>

BOOL CALLBACK allowAccess(HWND hwndChild, LPARAM lParam) {
    // Get the length of the text of the button
    int length = GetWindowTextLength(hwndChild);
    // Allocate buffer for button text
    TCHAR* buffer = new TCHAR[length + 1];
    // Get the button text
    GetWindowText(hwndChild, buffer, length + 1);

    CStringA label(buffer);

    if (label.Find("Allow access") != -1) {
        SendMessage(hwndChild, BM_CLICK, 0, 0);
        return FALSE;
    }
    delete[] buffer;
    return TRUE; // Continue enumerating child windows
}

//BOOL CALLBACK windowsSecurityAlert(HWND hwnd, LPARAM) {
//    TCHAR buf[1024]{};
//
//    GetClassName(hwnd, buf, 100);
//    std::cout << buf << endl;
//    if (!lstrcmp(buf, "Windows Security Alert"))
//    {
//        EnumChildWindows(hwnd, EnumChildProc, NULL);
//        //GetWindowText(hwnd, buf, 100);
//        //DWORD pid = 0;
//        //GetWindowThreadProcessId(hwnd, &pid);
//        //wcout << buf << " " << pid << endl;
//    }
//    return TRUE;
//}

//BOOL CALLBACK enumProc(HWND hwnd, LPARAM) {
//    TCHAR buf[1024]{};
//
//    GetClassName(hwnd, buf, 100);
//    if (!lstrcmp(buf, "Select gaganode"))
//    {
//        GetWindowText(hwnd, buf, 100);
//        DWORD pid = 0;
//        GetWindowThreadProcessId(hwnd, &pid);
//        wcout << buf << " " << pid << endl;
//    }
//    return TRUE;
//}

void startApp() {

    TCHAR filePath[1024];// = "C:\\node\\node\\gaganode.exe";

    if (SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, filePath)) {
        std::string homeFolder(pszPath);
        CoTaskMemFree(pszPath);
        return homeFolder;
    }

    // Replace "C:\\path\\to\\your\\executable.exe" with the path to your executable
    TCHAR filePath[1024]   =  "C:\\node\\node\\gaganode.exe";

    // Create process parameters
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);


    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_MINIMIZE;

    ZeroMemory(&pi, sizeof(pi));
    // Start the process
    if (!CreateProcess(
        NULL,                   // No module name (use command line)
        filePath,               // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,     // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi                     // Pointer to PROCESS_INFORMATION structure
    )) {
        std::cerr << "Error creating process: " << GetLastError() << std::endl;
    }
    else {

        // Optional: Wait for the process to finish
        WaitForSingleObject(pi.hProcess, 1000);

        //Find the window with the specified title
        HWND hWnd = FindWindow(NULL, "Windows Security Alert");
        if (hWnd != NULL) {
            EnumChildWindows(hWnd, allowAccess, NULL);
        }
    }
}

BOOL CALLBACK hideWindow(HWND hWnd, LPARAM lParam) {
    TCHAR windowTitle[256];
    GetWindowText(hWnd, windowTitle, sizeof(windowTitle));
    CString title(windowTitle);

    if (title.Find("gaganode.exe") != -1) {
        cout << "hiding : " << ShowWindow(hWnd, SW_HIDE) << endl;
        return false;
    }

    return TRUE; // Continue enumeration
}

DWORD GetProcessIdByName() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &entry)) {
        do {
            if (lstrcmp(entry.szExeFile, "gaganode.exe") == 0) {
                CloseHandle(hSnapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &entry));
    }

    CloseHandle(hSnapshot);
    return 0;
}


int main() {
    DWORD processId = GetProcessIdByName();
    if (processId == 0) {
        //Hide
        startApp();
        EnumWindows(hideWindow, 0);
    }
    else {
        cout << "Already running";
    }
  
}

//std::string GetUserHomeFolder() {
//    TCHAR;
//    if (SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &pszPath)) {
//        std::string homeFolder(pszPath);
//        CoTaskMemFree(pszPath);
//        return homeFolder;
//    }
//    else {
//        return "";
//    }
//}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
