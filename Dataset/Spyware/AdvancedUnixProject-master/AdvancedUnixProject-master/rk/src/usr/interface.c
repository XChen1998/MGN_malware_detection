#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/module.h>
#include <unistd.h>

#define TARGET_SYSCALL "sys/sc_example"

int main( int argc, char* argv[] )
{
	int modid;
	int syscall_num;
	struct module_stat stat;

	if( argc != 2)
	{
		printf( "USAGE:\n%s <string>\n", argv[0] );
		exit(0);
	}
	
	/*Find our desired Module*/
	modid = modfind( TARGET_SYSCALL );
	if( modid == -1)
	{
		printf("[MODFIND] error looking for %s\n", TARGET_SYSCALL);
		perror("[MODFIND] "); 
		exit(0);
	}
	
	/*Determine our syscall offset */
	stat.version = sizeof(stat);
	if( modstat( modid, &stat) == -1)
	{
		printf("[Main] Modstat failded for %d\n", modid);
		exit(0);
	}	
	syscall_num = stat.data.intval;
	printf("[Main] Target systemcall: %d\n", syscall_num);

	return( syscall( syscall_num, argv[1] ) );
} 
