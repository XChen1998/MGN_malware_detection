#include "core.h"


int launch(ModEntry* entrypoints, struct ModStat** mst_list, int nb_mods){

    int i;
    for (i = 0; i < nb_mods; i++){
        int exit_code = entrypoints[i](&mst_list[i]);
        if (exit_code == -1) return -1;
    }

    return 0;
}

