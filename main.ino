/*  
*   CEED Flappy Bird Arcade Machine!
*   --------------------------------
*   Files:
*   main.ino (the main loop code)
*   sensor.cpp (sensor updates)
*   screen.cpp (screen manager)
*   button.cpp (button observer)
*   conveyor_belt.cpp (belt manager)
*   pipes.cpp (pipe manager)
*
*   STATUS: NOT WORKING (IN PROGRESS)
*/  


#define INCREM_FREQUENCY 10000 // we should increment the score every 'INCREM_FREQUENCY' loop iterations
#define MAX_SCORE 999 // the maximum game score

enum game_state{
  WAITING; // we're waiting for the game to start
  SETUP; // we're adjusting everything to then start the game
  PLAYING; // We're currently playing the game
  GAME_OVER // we lost :(
}


// GAME VARIABLES
game_state curr_state = WAITING; // the current game state
int frequency = INCREM_FREQUENCY; // how often we should increment the score
int curr_score = 0; // the current score in the game (we cap it out at 999)


// FUNCTIONS
void score_increm() {curr_score = curr_score + 1;} // increment score by 1

void score_reset() {curr_score = 0;} // reset the score


//SETUP
void setup() {
  // put your setup code here, to run once:
  curr_state = WAITING;
}


// GAME LOOP
void loop() {
  // put your main code here, to run repeatedly:
  switch(curr_state) {
    case WAITING:

      // loop the intro animation

      // reset score

      score_reset();

      // look for a button input to go to SETUP
      if(/*button pressed*/false){
        curr_state = SETUP;
      }

      break;
    
    case SETUP:
      // adjust any values if need be

      // countdown to start like a '3,2,1, go!'-esque

      // at end, go to 'PLAYING'
      curr_state = PLAYING;
      break;

    case PLAYING:

      // chain is moving

      // pipes are moving

      // increment the score every second-ish
      if (frequency >= INCREM_FREQUENCY) {
        frequency = 0; // reset to minimum frequency value
        score_increm(); // increment the score
      }
      frequency = frequency + 1; // decrement the frequency

      // if we detect a sensor change from bg to pipes, go to GAME_OVER
      if(/*sensor collision*/false){
        curr_state = GAME_OVER;
      }

      break;

    case GAME_OVER:

      // display final score

      // reset pipes

      break;
  }
}
