# define _BSD_SOURCE
# define RED          "\x1b[31m"
# define GREEN        "\x1b[32m"
# define YELLOW       "\x1b[33m"
# define BLUE         "\x1b[34m"
# define BOLDBLACK    "\033[1m\033[30m"
# define BOLDRED      "\033[1m\033[31m"
# define BOLDGREEN    "\033[1m\033[32m"
# define BOLDYELLOW   "\033[1m\033[33m"
# define ANSI_RESET   "\x1b[0m"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <linux/limits.h>               // to use PATH_MAX based on the working machin's Linux
void myError(char* err);                 // Handels different kinds of errors
void readable(char* cwd);

/* ********** MAIN FUNCTION ********** */
int main(int argc, char* argv[])
{
  char cwd[PATH_MAX];                    // Current Work Directory

  if (argc == 1)                         // if no additional argument, then list for the current path (getcwd)
    getcwd(cwd, PATH_MAX);               // Read the Current Work Directory path into cwd 
  else if (argc == 2)                    // if one additional argument, then list for the current path
  {
    if (argv[1][0] == '/')               // if an Absolute path
      strcpy(cwd, argv[1]);              // It is a pathname
    else                                 // if a Relative path
      strcat( strcat(getcwd(cwd, PATH_MAX),"/"), argv[1]); // cwd + / + argv[1]
  }
  else                                   // if more than 1 additional command line argument
  {
    myError("argc");
    exit(1);
  }

  // To deal with user errors differently
  if (access(cwd, R_OK) != -1)           // if it is a valid path start functions
    readable(cwd);
  else
    myError("pathRead");                 // if file is not readable
	return 0;
}


/* ********** FUNCTIONS ********** */

/* ********** TRAVERSE READABLES ********** */
void readable(char* cwd)                    // Traverses the directories and their files
{
  struct stat temp;
  struct stat* tempPtr = &temp;             // gives me a pointer to my current working directory
  if ( lstat(cwd, tempPtr)==-1 || access(cwd, R_OK)==-1 )
    return;
  else
  {
    if (S_ISREG(tempPtr->st_mode))          // if it is a Regular File path
      printf("%s\n", cwd);
    else if (S_ISDIR(tempPtr->st_mode))     // if it is a Directory
    {
      chdir(cwd);                           // get into the current directory
      struct dirent* dirStr;
      DIR* dir = opendir(".");
      if (dir != NULL)
      {
        while ( (dirStr=readdir(dir)) != NULL )
        {
          if (strcmp(dirStr->d_name, ".")!=0 && strcmp(dirStr->d_name, ".."))      // if . or .. do nothing
            readable (strcat(strcat(getcwd(cwd, PATH_MAX),"/"), dirStr->d_name));   // cwd + / + dirStr->d_name
        }
        chdir("..");                        // go one step back and continue
        closedir(dir);
      }
    }
  }
  return;
}

/* ********** ERROR HANDELING ********** */
void myError(char* err)
{
  printf (BOLDRED  "ERROR: "  ANSI_RESET);
  if (strcmp(err, "argc")==0)               // if more than 2 command line arguments
    printf ("\tImproper use of command line arguments.\n");
  else if (strcmp(err, "pathRead")==0)      // if the path is not readable
    printf ("\tUnable to read the path. Please check your path and try again\n");
}

