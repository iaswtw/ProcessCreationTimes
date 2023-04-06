#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdio.h>
#include <timeapi.h>


int _tmain(int argc, TCHAR* argv[])
{
    const int numProcesses = 50;

    STARTUPINFO si[numProcesses] = { 0 };
    PROCESS_INFORMATION pi[numProcesses] = { 0 };
    DWORD startTime, endTime;

    if ((argc == 2) &&
        (wcscmp(argv[1], _T("-h")) == 0)
    )
    {
        printf("Usage: %S [external process cmdline]\n", argv[0]);
        return 0;
    }

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    for (int i = 0; i < numProcesses; i++)
    {
        si[i].cb = sizeof(si[i]);
    }

    startTime = timeGetTime();

    const int cmdLineSize = 1024;
    TCHAR cmdLine[cmdLineSize];

    printf("\nCreating %d processes with command line '%S'...\n\n", numProcesses, argv[1]);

    
    BOOL isHelloWorldExe = false;


    // Assume hello world executable if one isn't specified on command line
    if (wcsstr(argv[1], _T("HelloWorld.exe")) != NULL)
    {
        isHelloWorldExe = true;
    }

    // Create child processes as fast as we can.
    // Don't wait for any process to finish while this is being done.
    for (int i = 0; i < numProcesses; i++)
    {
        if (isHelloWorldExe)
        {
            _snwprintf_s(cmdLine, cmdLineSize - 1, _T("%s %d"), argv[1], i+1);
        }
        else
        {
            _snwprintf_s(cmdLine, cmdLineSize - 1, _T("%s"), argv[1]);
        }

        // Start the child process. 
        BOOL success = CreateProcess(NULL,   // No module name (use command line)
            cmdLine,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si[i],         // Pointer to STARTUPINFO structure
            &pi[i]          // Pointer to PROCESS_INFORMATION structure
        );

        if (!success)
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return 1;
        }
    }
    endTime = timeGetTime();
    
    // Wait until all child processes exit.
    for (int i = 0; i < numProcesses; i++)
    {
        WaitForSingleObject(pi[i].hProcess, INFINITE);

        // Close process and thread handles. 
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }


    printf("Time taken to create %d processes = %d ms\n", numProcesses, endTime - startTime);
    printf("Average = %d ms / process\n", ((endTime - startTime) + (numProcesses / 2)) / numProcesses);

    return 0;
}