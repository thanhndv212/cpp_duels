#include <gorilla/gorilla_game.h>

namespace gorilla {

GorillaGame::GorillaGame()
{
  // can be commented if you run the python GUI on your own during testing
  runGUI();

  // prepare init message

  initMsg init_msg;
  init_msg = gameSet();
  init_msg.sendToGUI("player 1", "player 2");


  // prepare first display message
  // display.field = ...


  display.sendToGUI();
}
initMsg GorillaGame::gameSet(){
    initMsg init_msg;
    int bottomLine = 335;
    init_msg.x1 = rand()%100 + 100;
    init_msg.x2 = rand()%300 + 220;
    init_msg.radius = 0;
    for ( int i=0; i<10;i++)
    {
        int height = rand()%150 +50;
      for(int k=0;k<64;k++)
      {
        init_msg.yb[i*64 + k] =   height;
      }
    }
    init_msg.y1 = bottomLine - init_msg.yb[init_msg.x1];
    init_msg.y2 = bottomLine - init_msg.yb[init_msg.x2];
    return init_msg;
};

feedbackMsg GorillaGame::getFeedback()
{
  // whatever data we need to communicate to the player
  feedbackMsg msg;

  return msg;
}

void GorillaGame::play(inputMsg msg)
{
  // update game state according to player input



  // let the AI play
  playAI();

  // update display communicate to GUI

  display.sendToGUI(winner_);
}

void GorillaGame::playAI()
{



}
}
