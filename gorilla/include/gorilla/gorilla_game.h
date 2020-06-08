#ifndef GORILLAGAME_H
#define GORILLAGAME_H

#include <gorilla/msg.h>

namespace gorilla {

class GorillaGame
{
public:
  // initialize the game and send to GUI
  GorillaGame();

  feedbackMsg getFeedback(initMsg initMsg, inputMsg input, int turn, double traveling_time);

  initMsg gameSet();

  inputMsg getInput();

  void play(initMsg initMsg, inputMsg msg, displayMsg display, int turn);

  bool over() const
  {
    return winner_ != 0;
  }

  int winner() const
  {
    return winner_;
  }

private:

  void playAI();
  displayMsg display;
  int winner_ = 0;


};
}
#endif // GORILLAGAME_H
