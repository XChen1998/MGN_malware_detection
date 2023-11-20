#ifndef _ERROR_H
#define	_ERROR_H

#define ERROR_MSG(msg, format...) fprintf(stderr,"(%s:%i)" msg "\n", \
    __FILE__, __LINE__, ##format)

typedef enum {
    RS_ERROR_OK,
    RS_ERROR_MALLOC,
    RS_ERROR_REALLOC,
    RS_ERROR_RESOLVE,
    RS_ERROR_CONNECT,
    RS_ERROR_LOAD_LIBRARY,
    RS_ERROR_CREATE_PROCCESS,
    RS_ERROR_SHELL_EXECUTE,
    RS_ERROR_MISSING_ARGUMENTS,
    RS_ERROR_GET_INTERFACES,
    RS_ERROR_OVERFLOW,
    RS_ERROR_SOCKET
} RS_ERROR;

#endif	/* _ERROR_H */

