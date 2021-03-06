#ifndef ATTACHABLE_DEBUGGER
#define ATTACHABLE_DEBUGGER 

#include <sys/prctl.h>

/***
 * returns non-zero on any error, more likely than not with 
 * errno set.
 *
 * See
 *   /etc/sysctl.d/10-ptrace.conf
 * on Ubuntu systems.  The ability to attach to processes for debugging
 * is disabled by default.  We could petition CCIT to change the conf value,
 * but then students will still run into this problem on personal machines.
 * 
 * Another approach is this function.  When called it looks at *all* PIDs
 * belonging to the process user and explicitely allows their ptrace
 * connection. As long as the debugger (gdb, gdbtui, ddd) is running *before*
 * we start this process, we should be all good.
 *
 * Call this function at the top of main().
 */
int attachable_debugger( void );

#endif
