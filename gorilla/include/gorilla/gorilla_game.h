#ifndef GORILLAGAME_H
#define GORILLAGAME_H

#include <gorilla/msg.h>

namespace gorilla {

class GorillaGame
{
public:
  // initialize the game and send to GUI
  GorillaGame();

  feedbackMsg getFeedback();
  initMsg gameSet();
  void play(inputMsg msg);

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
