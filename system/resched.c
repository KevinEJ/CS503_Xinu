/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;


/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];


    //EJ_Lab1 
#if EJ_Lab1 
    //////  Set current group to init_priority 
     if( ptold -> p_Group == PS1 ){
        PS1_Priority = PS1_init_Priority ; 
    } else if ( ptold -> p_Group == PS2 ){
        PS2_Priority = PS2_init_Priority ; 
    } else{
        kprintf( " @@@ ERROR: Current Process does not belong to any group @@@ \n" ) ; 
    }

    //////  Update Priority 
    //////  Global Variable -> PS1_Priority , PS2_Priority 
         
    qid16 curr = firstid( readylist );                                 // First pid in Readylist   ; 
	while ( curr != queuetail(readylist) && queuetab[curr].qnext != queuetail(readylist) ) {     // Search from firstid to lastid 
        if( curr == NULLPROC ){ 
            //kprintf ( "Ideally you would not see this message \n" ) ; 
            curr = queuetab[curr].qnext;
            continue ; 
        } 
        if( curr == currpid ){
            kprintf ( "@@@ ERROR: currpid is in ReadyList @@@ \n" ) ; 
            curr = queuetab[curr].qnext;
            continue ; 
        }

        if( (&proctab[curr]) -> p_Group == PS1 ) 
            PS1_Priority ++ ; 
        else if( (&proctab[curr]) -> p_Group == PS2)
            PS2_Priority ++ ; 
        else{ 
            kprintf ( "@@@ ERROR: a process is in neither in PS1 nor PS2 \n" ) ;  
            kprintf ( "@@@ ERROR: a process [%d] [%s] is in neither in PS1 nor PS2 \n" , curr , proctab[curr].prname ) ;  
        }
        curr = queuetab[curr].qnext;
	} 

    //////  Select Group 
    int current_Group = PS1_Priority >= PS2_Priority ? PS1 : PS2 ;  
    //int current_Group = PS1 ;  
    
    //////  Update Process Priority 
    //uint64 T = getticks () ; 
    pri16 T = Clock_ticks ; 
    //kprintf( " Log: T          = %d , \n"  , T ) ; 
    //kprintf( " Log: t          = %d , \n"  , ptold->p_T_LastSche ) ; 
    //kprintf( " Log: T - t      = %d , \n"  , T - ptold->p_T_LastSche ) ; 
    if( ptold->p_rate == 0 ){   // For special cases: NULL, STARTUP, MAIN 
        ptold -> p_pi = INT_MAX ;
    }else if( ptold -> p_rate == -1 ){
        ptold -> p_pi = 0 ;
    }else{
        ptold -> p_pi = ptold -> p_pi +  (int)( ( T - ( ptold -> p_T_LastSche) ) * 100 ) / (int)(ptold -> p_rate)    ; 
        // How to get T-old //T = getticks()  
    }
    ptold -> prprio = INT_MAX - ( ptold -> p_pi ) ;  
    
    kprintf ( " current process from group [%d], id [%d],  name = [%s] , prio = [%d] \n", 
                                            ptold->p_Group , currpid,  ptold->prname , ptold->prprio) ;
    ////// Error check //////
    if( ptold -> p_pi < 0 )
        kprintf( " @@@ ERROR : pi less than 0 @@@ \n" ) ;  
    if( ptold -> prprio < 0 )
        kprintf( " @@@ ERROR : prprio less than 0 @@@ \n" ) ;  


    //////  Put current process in readylist if it remains eligible 
    if ( ptold->prstate == PR_CURR ){
        ptold -> prstate = PR_READY ; 
        insert(currpid, readylist, ptold->prprio);
    }

    //////  Select Process
    curr = firstid( readylist );                                 // First pid in Readylist   ; 
	while ( queuetab[curr].qnext != queuetail(readylist) ) {     // Search from firstid to lastid 
        if( (&proctab[curr])->p_Group == current_Group ){ 
            currpid = getitem(curr);  
            ptnew = &proctab[currpid];
	        ptnew->prstate = PR_CURR;
            break ;
        }
        curr = queuetab[curr].qnext ; 
    }
	if ( queuetab[curr].qnext == queuetail(readylist) ) {     // Search from firstid to lastid  
        //kprintf( " @@@ ERROR: no process in selected group !!! @@@ \n" ) ;
        //kprintf( " @@@ ERROR: You should not see this message !!!!! @@@ \n" ) ;
        currpid = dequeue (readylist) ;
	    ptnew = &proctab[currpid];
	    ptnew->prstate = PR_CURR;
    }
    //////   Update Pi = max(Pi, T)  
    ptnew -> p_pi = ( ptnew -> p_pi > T )? ptnew->p_pi : T ;
    ptnew -> prprio = INT_MAX - ( ptnew -> p_pi ) ;  
    //////   Update t = getticks() ;
    ptnew -> p_T_LastSche = T ; 
    /////    Context Switch 
    //kprintf( " TIMING: T = %u , t = %u , T - t \n" , T , ptnew -> p_T_LastSche ) ; 
    kprintf ( "selected process from group [%d], id [%d],  name = [%s] , prio = [%d] \n", 
                                            ptnew->p_Group , currpid,  ptnew->prname , ptnew->prprio) ;
	preempt = QUANTUM;		/* Reset time slice for process	*/
    ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
            

    //EJ_Lab1
#else 
	// original code 
    if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
#endif
	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
