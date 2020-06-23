#include <gorilla/msg.h>
#include <gorilla/gorilla_game.h>
#include <iostream>

using namespace gorilla;
using namespace std;

int main(int argc, char** argv)
{

     GorillaGame();
     //sendclose2GUI();                  // to send a exit message to GUI after running the whole program

//  runGUI();
//  initMsg init_msg;
//  feedbackMsg fb1_msg, fb2_msg;
//  inputMsg input;
//  displayMsg display;


//  bool newRound = true;
//  int winner(0);
 /* while(winner == 0)
  {
    // update game state including winner variable if any
    int first_turn = rand()%2+1;
    int turn = first_turn;

    // update display
    
    
    display.sendToGUI(winner);
    if(winner)
      break;
  }*/
  //std::cout << "Player " << winner << " has won!\n";
  return 0;
}
