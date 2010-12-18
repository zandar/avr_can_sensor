/* Functoins for executing FSM */

/*@{*/
#include "../include/fsm.h"

void init_fsm(struct fsm *fsm, state_fcn initial_state)
{
	fsm->current_state = initial_state;
	fsm->current_state(fsm, EVENT_ENTRY);
}

void run_fsm(struct fsm *fsm){
	fsm->last_state = fsm->current_state;		// set actual state
	fsm->current_state(fsm, EVENT_DO);		// change parameter
	
	if(fsm->last_state != fsm->current_state){ 	// if state was changed
		fsm->last_state(fsm, EVENT_EXIT); 	// finish the old state
		fsm->current_state(fsm, EVENT_ENTRY); 	// initialize the new state
	}
}

/*@}*/


