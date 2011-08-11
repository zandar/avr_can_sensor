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

/* Definitions of FSM parameters */

#ifndef FSM_H
#define FSM_H

/* events of each state of state machine */
enum event {
	EVENT_ENTRY,
	EVENT_DO,
	EVENT_EXIT
};

struct fsm;

/* pointer to function returning void and two input parametr */
typedef void (*state_fcn)(struct fsm *fsm, enum event my_event);

struct fsm {
	state_fcn current_state;          /* current state */
	state_fcn last_state;             /* last state */
	volatile char measurement_start;  /* measurement start flag*/
	volatile char send_samples;       /* send samples flag */
};

void init_fsm(struct fsm *fsm, state_fcn initial_state);
void run_fsm(struct fsm *fsm);

#endif
