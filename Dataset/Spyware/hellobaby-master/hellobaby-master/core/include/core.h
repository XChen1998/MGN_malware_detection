#ifndef CORE_H_
#define CORE_H_



struct ModStat {
    int type;
};
    
    
typedef int (*ModEntry)(struct ModStat**);

int launch(ModEntry* entrypoints, struct ModStat** mst_list, int nb_mods);

#endif
