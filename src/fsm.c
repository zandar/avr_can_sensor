/*
 * This file is part of avr_can_sensor.
 *
 * Avr_can_sensor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Avr_can_sensor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avr_can_sensor.  If not, see <http://www.gnu.org/licenses/>.
 */

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


