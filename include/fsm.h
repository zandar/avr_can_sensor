#ifndef FSM_H
#define FSM_H

// events of each state of state machine
enum event {
	EVENT_ENTRY,
	EVENT_DO,
	EVENT_EXIT
}; 

struct fsm;

typedef void (*state_fcn)(struct fsm *fsm, enum event my_event);//pointer to function returning void and two input parametr

struct fsm {
	state_fcn current_state; 	        // current state
	state_fcn last_state;			// last state
	volatile char measurement_start;
	volatile char send_samples;
	volatile char sensor_ready;
};

void init_fsm(struct fsm *fsm, state_fcn initial_state);
void run_fsm(struct fsm *fsm);

#endif
