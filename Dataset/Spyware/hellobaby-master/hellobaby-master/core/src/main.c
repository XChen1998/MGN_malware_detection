#include <stdio.h>
#include "core.h"

/**
 * The file "includes.h" is created at build time. It contains
 * all the include directives of all the main headers of the 
 * modules activated which names must be the same as the name
 * of the module (e.g. startup ==> startup.h).
 */

#include "includes.h"




int main(void){


    /**
     * The macros NB_MODULES and ENTP_LIST are defined using the
     * values of the config file and passed by the preprocessor.
     *
     * - NB_MODULES contains the number of modules used.
     * - ENTP_LIST contains a comma-separed list of the entrypoints
     *   of each module. By convention each entry must be named by
     *   following this template: launch_<monule type>_<module name>
     *   (e.g. the name of the entry function of the module `startup`
     *   into the section `spread` would be: launch_infect_startup.
     */
     
    #if defined NB_MODULES && defined ENTP_LIST

    struct ModStat* mst_list[NB_MODULES];

    ModEntry entrypoints[NB_MODULES] = { ENTP_LIST };

    #else

        #error "NB_MODULES and ENTP_LIST not defined"

    #endif

    if ( launch(entrypoints,mst_list, NB_MODULES) == -1){
        fprintf(stderr,"Fail");
    }

    return 0;

}
    
