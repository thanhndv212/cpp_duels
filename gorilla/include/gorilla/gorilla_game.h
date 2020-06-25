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
  inputMsg getInput_AI(initMsg initMsg, int pre_xb2, int pre_yb2, int pre_xb, int pre_yb,double pre_vel2, double pre_angle2, double pre_vel, double pre_angle, int iter, int turn);

  void play(initMsg initMsg, inputMsg msg, displayMsg display, int turn);

  bool over() const
  {
    return winner_ != 0;
  }

  int winner() const
  {
    return winner_;
  }
  int pre_xb = 0;
  int pre_yb = 0;
  int pre_xb2 = 0;
  int pre_yb2 = 0;
  double pre_vel = 50;
  double pre_angle = 85;
  double pre_vel2 = 50;
  double pre_angle2 = 85;
  const double k = 0.2;
private:

  void playAI();
  displayMsg display;
  int winner_ = 0;



};
}
#endif // GORILLAGAME_H
