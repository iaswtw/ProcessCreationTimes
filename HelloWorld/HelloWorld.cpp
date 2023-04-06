#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, TCHAR *argv[])
{
    printf("Hello World");
    if (argc > 1)
    {
        _tprintf(_T(" %s"), argv[1]);
    }
    
    printf("!\n");
}
