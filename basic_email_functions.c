#include <dirent.h>
#include <errno.h>

#include "basic_email_functions.h"

int checkDomain(){

    DIR* dir = opendir("mydir");
    if (dir)
    {
        return 1;
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
        return 0;
    }
    else
    {
        return;
    }
}
int checkUser(){

}