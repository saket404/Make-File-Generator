/**************************************************
* Main.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "main.h"
#include "generator.h"
#include <time.h>
#if defined(WIN32) || defined(_WIN32)
#include <crtdbg.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
#elif defined(linux) || defined(__linux)
#include <unistd.h>
#endif

int main()
    {
    return DoGenerate();
    }

int DoGenerate()
    {
    char choice[32];
    char srcDir[512];
    char stop = 0;
    int userChoice;
    int generated;
    size_t len;

    while (!stop)
        {
        ShowMainMenu();
        /* Get user choice */
        if (fgets(choice, sizeof(choice), stdin))
            sscanf(choice, "%d", &userChoice);
        else
            userChoice = -1;

        switch (userChoice)
            {
            case 1:
                printf("Source code directory: ");
                if (!fgets(srcDir, sizeof(srcDir), stdin))
                    srcDir[0] = 0;
                len = strlen(srcDir);
                /* Remove trailing newline character */
                if (len > 0 && srcDir[len - 1] == '\n')
                    srcDir[len - 1] = 0;
                if (SourceDirExists(srcDir))
                    {
                    generated = Generator_GenerateMakeFile(srcDir);
                    if (generated > 0)
                        printf("%d executable(s) generated.\n", generated);
                    else if (generated == 0)
                        printf("No executable generated.\n");
                    else
                        printf("An error occurred. No executable file generated.\n");

                    printf("Thank You for using MakeFile Generator\nPlease check your directory for the makefile.\n");
                    }
                else
                    printf("Directory doesn't exist. Please try again!\n");
                break;
            case 2:
				printf("-----------------------How to use Makefile Generator----------------------------\n");
    			printf("1.As you enter the program you will choose the first option of creating the make \n");
    			printf("  file. The program will then ask you to enter your directory. You will need to  \n");
    			printf("  check ur username first. Write 'pwd' in the terminal which will give you the   \n");
				printf(" the directory to enter first. Example /home/(your username). Then you can write \n");
				printf(" the rest of the part which tells which directory your files are in. For example \n");
				printf("  /home/(your username)/Desktop/project. If your directory exists and it has the \n");
				printf("  files, the program will automatically scan and create a makefile which will be \n");
				printf("  in your directory with your files.											 \n");	
    			printf("--------------------------------------------------------------------------------\n");
                break;
			case 3:
				stop = 1;
				break;
            default:
                printf("Invalid choice.\n");
                break;
            }
        }

    return 0;
    }

void ShowMainMenu()
    {
    printf("-----------------------Welcome to MakeFile Generator----------------------------\n");
    printf("                          1. Generate make file\n");
    printf("                          2. Userguide\n");
    printf("                          3. Exit\n");	
    printf("--------------------------------------------------------------------------------\n");
    printf("Your choice (1-3): ");
    }

/*******************************************************************************
Check whether the source directory exists
Input:
	srcDir: source directory to check
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int SourceDirExists(const char *srcDir)
    {
    if (srcDir[0] == 0)
        return 0;	/* Empty directory */

    /* Check directory existence here */
#if defined(WIN32) || defined(_WIN32)
    return PathIsDirectory(srcDir);
#elif defined(linux) || defined(__linux)
    return (!access(srcDir, F_OK));
#else
#error Unknown platform
#endif
    }
