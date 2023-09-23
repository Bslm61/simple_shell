#include "custom_shell.h"

int main(int argc, char **argv)
{
    ShellInfo shellInfo[] = { SHELL_INFO_INIT };
    int fileDescriptor = 2;

    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (fileDescriptor)
         : "r" (fileDescriptor));

    if (argc == 2)
    {
        fileDescriptor = open(argv[1], O_RDONLY);
        if (fileDescriptor == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                printErrorMessage(argv[0]);
                printErrorMessage(": 0: Can't open ");
                printErrorMessage(argv[1]);
                printErrorMessage("\n");
                printErrorMessage(BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        shellInfo->readFileDescriptor = fileDescriptor;
    }
    populateEnvironmentList(shellInfo);
    readShellHistory(shellInfo);
    runCustomShell(shellInfo, argv);
    return (EXIT_SUCCESS);
}
