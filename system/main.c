/*  main.c  - main */

#include <xinu.h>

void sample_proc_cpu() {
        int i, j;
        int LOOP1 = 10; 
        int LOOP2 = 5000000;
        //struct procent *pr = &proctab[currpid];
        int v = 0;
        for (i=0; i<LOOP1; i++) {
                for (j=0; j<LOOP2; j++) {
                        // Note: this loop consumes significant CPU cycles.
                        v += i * j;
                }   
                //kprintf("[1] PID: %d, Loop: %d \n", currpid, i);
                kprintf("[1] PID: %d, Loop: %d , v: %d \n", currpid, i , v );
        }   
        kprintf("[1] ===== CPU BOUNDED PID %d ends with output = %d \n", currpid , v );
}
void sample_proc_cpu_2() {
        int i, j;
        int LOOP1 = 10; 
        int LOOP2 = 5000000;
        //struct procent *pr = &proctab[currpid];
        int v = 0;
        for (i=0; i<LOOP1; i++) {
                for (j=0; j<LOOP2; j++) {
                        // Note: this loop consumes significant CPU cycles.
                        v += i * j;
                }   
                kprintf("[2] PID: %d, Loop: %d , v: %d \n", currpid, i , v );
        }   
        kprintf("[2] ===== CPU BOUNDED PID %d ends with output = %d \n", currpid , v );

}
process main() {
	//int i;
	resched_cntl(DEFER_START);
    //chgprio( PS1 , 100 ) ; 
        //for (i = 0; i < 2; i++) {
        //    resume(create(sample_proc_cpu, 1024, PS2, 40 + i*10 , "cpu-intense", 0, NULL));
        //}   
        resume(create(sample_proc_cpu, 1024, PS2, 40   , "  A  ", 0, NULL));
        resume(create(sample_proc_cpu, 1024, PS2, 50   , "  B  ", 0, NULL));
        resume(create(sample_proc_cpu, 1024, PS2, 50   , "  B  ", 0, NULL));
        resume(create(sample_proc_cpu, 1024, PS2, 50   , "  B  ", 0, NULL));
        resume(create(sample_proc_cpu, 1024, PS2, 50   , "  B  ", 0, NULL));
    resched_cntl(DEFER_STOP);
    int v     =  0 ; 
        for (int i=0; i< 100; i++) {
            for (int j=0; j< 1000000; j++) 
                v += i * j;
        }
	resched_cntl(DEFER_START);
        resume(create(sample_proc_cpu_2, 1024, PS2, 30 , "  C  ", 0, NULL));
        resume(create(sample_proc_cpu_2, 1024, PS2, 30 , "  C  ", 0, NULL));
        resume(create(sample_proc_cpu_2, 1024, PS2, 30 , "  C  ", 0, NULL));
        resume(create(sample_proc_cpu_2, 1024, PS2, 30 , "  C  ", 0, NULL));
        resume(create(sample_proc_cpu_2, 1024, PS2, 60 , "  D  ", 0, NULL));
    resched_cntl(DEFER_STOP);
    

}




/*



void myprocess1(void) {
    while(TRUE){
        kprintf("I'm Process 1 !\n");
    }
}

void myprocess2(void) {
    while(TRUE){
        kprintf("I'm Process 2 !\n");
    }
}

process	main(void)
{

	// Run the Xinu shell //
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, WCONSOLE));
	//resume(create(shell, 8192, 50, "shell", 1, WCONSOLE));
	//resume(create(myprocess, 4096, 25, "Hello world", 0));

	// Wait for shell to exit and recreate it //

	while (TRUE) {
		
        receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, WCONSOLE));
	    
    }
	return OK;
    
}*/
