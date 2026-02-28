/**
* Test script for the stepper motor
*/

/*
  suggested wiring for motor driver
    wire from UNO_5V to loc_POW
    wire from UNO_GND to loc_GND
    wire from UNO_DIGI_PIN_A to loc_STEP
    wire from UNO_DIGI_PIN_B to loc_DIR

    WARNING: Can blow up when using long power leads,
      use a safety capacitor.
*/

int steps_per_rev = 200; // approx ~1.8' per step

/**
* the number of revolutions it takes to get from the bottom of the bar, to the top.
* NEEDS TO BE MEASURED
*/
const int revs_per_bar = 2000;

/**
* Current in Amperes. Meant for full-step mode.
* Max ~1.7 (unloaded or loaded)
*/
float suggested_curr = 1.7;

// each of the five direction pins in order
int direction_pins[5] = {2, 31, 31, 31, 31};
//5 step pins, in order
int step_pins[5] = {3, 31, 31, 31, 31};

/**
* the index of the pipes we will adjust.
* MUST BE INTEGER in {0,1,2,3,4}
*/
int pipes_to_adj = 0;


/**
* the desired revolutions away from the bottom for pipes_to_adj. Position 0 = top.
* MUST BE BETWEEN 0 <= x <= revs_per_bar
*/
float desired_pos = 2000.0;

/*
* stored floats representing the current revolutions away from the bottom of the bar
* for each of the 5 pipes.
* MUST BE LESS THAN REVS_PER_BAR
*/
float pipe_positions[5] = {0, 0, 0, 0, 0};

/*
* delay_time is the time between every step, in microseconds. 
* Minimum ~400 unloaded, ~450 with bar inserted.
* MUST BE NONNEGATIVE
*/
int delay_time = 450;

/**
* the time it takes for a pipe to do a full revolution about the track (seconds).
* MUST BE NONNEGATIVE
*/ 
float pipe_revolution_time = 10.0;



// signal we wait for to change a pipe set
bool signal_received = true;


void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 5; i++) {
    if(step_pins[i] != 31){
      pinMode(step_pins[i], OUTPUT);
      pinMode(direction_pins[i], OUTPUT);
    }
  }

  // Serial.begin(115200);
}

// PLACEHOLDER
void loop() {
  desired_pos = random(revs_per_bar + 1);

  // // offset by moving the pipes
  int adjust_time = 0;

  if(signal_received) {
    adjust_time = change_pos_time(pipes_to_adj, desired_pos);
    change_pos(pipes_to_adj, desired_pos);
    signal_received = false;
  }


  // // change the next pipe
  // pipes_to_adj = (pipes_to_adj < 4) ? pipes_to_adj + 1 : 0;

  // // wait one-fifth of the time before adjusting the next pipe, minus any offset
  // delay((pipe_revolution_time * 1000 / 5) - adjust_time);

  // // NOTE: MUST DECREASE THIS TIME BY THE TIME IT TAKES TO ADJUST THE PIPES THEMSELVES
  // // CURRENTLY OFFSETS BY HOWEVER LONG IT TAKES TO ADJUST THE PIPES
}


/**
* changes the position of the pipes pipe_index, from pipe_positions[pipe_index],
* into desired_pos
* @param pipe_index pipe number to use. Must be on the interval [0, 4]
* @param desired_pos final position to go to. Must be on the interval [0, revs_per_bar (a global constant)]
* @return time, in ms, taken to move
*/
int change_pos(int pipe_index, float desired_pos) {

  // time taken to move the pipes, in milliseconds
  int time_elapsed = 0;

  // bounded desired_pos to [0f, revs_per_bar]
  float adj_pos = (desired_pos <= 0) ? 0 : (desired_pos >= revs_per_bar) ? revs_per_bar : desired_pos;
  
  // the difference between the desired position, and the current position
  float difference = (adj_pos - pipe_positions[pipe_index]);

  if (difference < 0) {
    // clockwise goes down
    digitalWrite(direction_pins[pipe_index], HIGH);
  }else if (difference > 0){
    // counter-clockwise goes up
    digitalWrite(direction_pins[pipe_index], LOW);
  }

  // the number of revolutions we must rotate to adjust
  float abs_diff = (difference < 0) ? -1 * difference : difference;

  // move one revolution, |difference| times
  for(int i = 0; i < (steps_per_rev * abs_diff); i++) {
    digitalWrite(step_pins[pipe_index], HIGH);
    delayMicroseconds(delay_time);
    time_elapsed += 1;
    digitalWrite(step_pins[pipe_index], LOW);
    // Serial.println(i);
    delayMicroseconds(delay_time);
    time_elapsed += 1;
  }

  // save the new position state
  pipe_positions[pipe_index] = adj_pos;

  // number of milliseconds taken to move
  return time_elapsed;
}


/**
* Returns the time in milliseconds elapsed to change the position of pipe `pipe_index` to `desired_pos`
* 
* @param pipe_index number of pipe to test
* @param desired_pos position to calculate from
* @return milliseconds to move the pipe
*/
int change_pos_time(int pipe_index, float desired_pos) {

  // time taken to move the pipes, in milliseconds
  int time_elapsed = 0;

  // bounded desired_pos to [0f, revs_per_bar]
  float adj_pos = (desired_pos <= 0) ? 0 : (desired_pos >= revs_per_bar) ? revs_per_bar : desired_pos;
  
  // the difference between the desired position, and the current position
  float difference = (adj_pos - pipe_positions[pipe_index]);


  // the number of revolutions we must rotate to adjust
  float abs_diff = (difference < 0) ? -1 * difference : difference;

  // move one revolution, |difference| times
  for(int i = 0; i < (steps_per_rev * abs_diff); i++) {
    time_elapsed += 1;
    time_elapsed += 1;
  }
  // number of milliseconds taken to move
  return time_elapsed;
}
