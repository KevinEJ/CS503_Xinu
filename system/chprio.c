/* chprio.c - chprio */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  chprio  -  Change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
pri16	chprio(
	  pid32		pid,		/* ID of process to change	*/
	  pri16		newprio		/* New priority			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	oldprio;		/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16) SYSERR;
	}
	prptr = &proctab[pid];
	oldprio = prptr->prprio;
	prptr->prprio = newprio;
	restore(mask);
	return oldprio;
}

// EJ_Lab1 
#if EJ_Lab1
pri16	chgprio(
	  int 		group,		// group to change	        //
	  pri16		newprio		// New priority			    //
	)
{
	intmask	mask;			// Saved interrupt mask		//
	pri16	oldprio;		// Priority to return		//

	mask = disable();
	

    if( group == PS1 ){
        oldprio             = PS1_init_Priority ; 
        PS1_init_Priority   = newprio ; 
    }else if( group == PS2 ) {
        oldprio             = PS2_init_Priority ; 
        PS2_init_Priority   = newprio ; 
    }else{
        kprintf( " @@@ ERROR: chgprio : invalid group \n" ) ; 
		restore(mask);
        return (pri16) SYSERR ; 
    }

	restore(mask);
    
    return oldprio;

}
#endif
