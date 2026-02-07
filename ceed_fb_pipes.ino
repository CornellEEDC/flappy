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

// the number of revolutions it takes to get from the bottom of the bar, to the top
// NEEDS TO BE MEASURED
int revs_per_bar = 20;

// meant for full-step mode
float suggested_curr = 1.4; // Amperes

int direction_pin = 2;
int step_pin = 3;

// the index of the pipes we will adjust
// MUST BE INTEGER in {0,1,2,3,4}
int pipes_to_adj = 0;


// the desired revolutions away from the bottom for pipes_to_adj
// MUST BE BETWEEN 0 <= x <= revs_per_bar
int desired_pos = 0.0;

// stored floats representing the current revolutions away from the bottom of the bar
// for each of the 5 pipes
// MUST BE LESS THAN REVS_PER_BAR
float pipe_positions[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

// delay_time is the time between every step, in microseconds
// MUST BE NONNEGATIVE
int delay_time = 1000;

// the time it takes for a pipe to do a full revolution about the track (seconds)
// MUST BE NONNEGATIVE
float pipe_revolution_time = 10.0;



// signal we wait for to change a pipe set
bool signal_received = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(step_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);
}

// PLACEHOLDER
void loop() {
  desired_pos = random(revs_per_bar + 1);

  // offset by moving the pipes
  int adjust_time = 0;

  if(signal_received) {
    adjust_time = change_pos(pipes_to_adj, desired_pos);
    signal_received = false;
  }


  // change the next pipe
  pipes_to_adj = (pipes_to_adj < 4) ? pipes_to_adj + 1 : 0;

  // wait one-fifth of the time before adjusting the next pipe, minus any offset
  delay((pipe_revolution_time * 1000 / 5) - adjust_time);

  // NOTE: MUST DECREASE THIS TIME BY THE TIME IT TAKES TO ADJUST THE PIPES THEMSELVES
  // CURRENTLY OFFSETS BY HOWEVER LONG IT TAKES TO ADJUST THE PIPES
}

// changes the position of the pipes pipe_index, from pipe_positions[pipe_index],
// into desired_pos
int change_pos(int pipe_index, int desired_pos) {

  // time taken to move the pipes, in milliseconds
  int time_elapsed = 0;

  // bounded desired_pos to [0f, revs_per_bar]
  float adj_pos = (desired_pos <= 0) ? 0 : (desired_pos >= revs_per_bar) ? revs_per_bar : desired_pos;
  
  // the difference between the desired position, and the current position
  int difference = (adj_pos - pipe_positions[pipe_index]);

  if (difference < 0) {
    // clockwise goes down
    digitalWrite(direction_pin, HIGH);
  }else if (difference > 0){
    // counter-clockwise goes up
    digitalWrite(direction_pin, LOW);
  }

  // the number of revolutions we must rotate to adjust
  int abs_diff = (difference < 0) ? -1 * difference : difference;

  // move one revolution, |difference| times
  for(int i = 0; i < (steps_per_rev * abs_diff); i++) {
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(delay_time);
    time_elapsed += 1;
    digitalWrite(step_pin, LOW);
    delayMicroseconds(delay_time);
    time_elapsed += 1;
  }

  // save the new position state
  pipe_positions[pipe_index] = adj_pos;

  // number of milliseconds taken to move
  return time_elapsed;
}
