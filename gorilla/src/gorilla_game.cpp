#include <gorilla/gorilla_game.h>
#include <gorilla/msg.h>
#include <iostream>
#include <math.h>

#define PI 3.14159265

using namespace std;
namespace gorilla {

GorillaGame::GorillaGame()
{
  // can be commented if you run the python GUI on your own during testing
  runGUI();

  // prepare init message

  initMsg init_msg;
  init_msg = gameSet();
  init_msg.sendToGUI("player 1", "player 2");

  inputMsg input;
  feedbackMsg fb1_msg, fb2_msg;
  displayMsg display;

  //prepare input and display input message
  int winner = 0;
  int i = 0;
  int first_turn =1;
  int turn = first_turn;

  while(winner == 0)
  {
      input = getInput();
      //play(init_msg, input, display, turn);
      if(turn==1)
      {
          turn = 2;
      }
      else
          turn = 1;
      i++;
      if(i==3)
      {
          winner = 1;
      }
      if(winner)
          break;
  }
   cout<<"player "<<winner<<" has won!\n";

  //display.sendToGUI();
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

inputMsg GorillaGame::getInput()
{
    //get input from player
    inputMsg input;
    cout<<"Enter a velocity : ";
    cin>>input.force;
    cout<<"You have entered "<<input.force<<'\n';
    cout<<"Enter a angle: ";
    cin>>input.angle;
    cout<<"You have entered: "<<input.angle<<'\n';
    return input;
}

feedbackMsg GorillaGame::getFeedback(initMsg initMsg, inputMsg input, int turn, double traveling_time)
{
  // whatever data we need to communicate to the player
  double gravity = 10;
  feedbackMsg msg;
  if(turn ==1 ){
      msg.x = initMsg.x1;
      msg.y = initMsg.y1;
      msg.xo = initMsg.x2;
      msg.yo = initMsg.y2;
      msg.wind = rand()%2 - 2;
  }
  if(turn==2)
  {
      msg.x = initMsg.x2;
      msg.y = initMsg.y2;
      msg.xo = initMsg.x1;
      msg.yo = initMsg.y1;
      msg.wind = rand()%2 - 2;
  }
  //starting position of banana
  int start_x = msg.x;
  int start_y = msg.y;
  //velocity and angle
  double angle = input.angle;
  double vel = input.force;
  double cosine = cos(angle*PI/180);
  double sine = sin(angle*PI/180);
  int wind = msg.wind;

  int banana_x;
  int banana_y;
  //handling time
  double t = traveling_time;
  //update banana's position
  banana_x = int(start_x + vel*cosine*t + (1/2)*wind*t*t);
  banana_y = int(start_y + vel*sine*t - (1/2)*gravity*t*t);

  msg.xb = banana_x;
  msg.yb = banana_x;
  //cout<<"pass this first!\n";
  return msg;
}

void GorillaGame::play(initMsg initMsg, inputMsg input, displayMsg display, int turn)
{

  cout<<"This is player"<<turn<<"'s turn!\n";

  cout<<"Player has shoot a banana with a velocity of "<<input.force<<" at a angle of "<<input.angle<<'\n';

  //How to handle timer to calculate trajectory of banana and update display to GUI???

  double t = 1;
  //main play
   feedbackMsg fb_msg;

   int banana_x;
   int banana_y;
   bool HIT = 0;
   bool inplay = 1;
   while (not HIT && inplay)
     {
         fb_msg = getFeedback(initMsg, input, turn, t);
         banana_x = fb_msg.xb;
         banana_y = fb_msg.yb;
         //check if (NOT inplay) --> break loop, reverse turn; message out "banana went out of range"
         display.x = banana_x;
         display.y = banana_y;
         //check if hit targeted gorilla --> HIT = 1; winner = turn;
         //if(HIT){over();winner()}
         winner_ = 1;
         //display.sendToGUI(winner());
         t+=0.01;
         HIT = 1;
         //cout<<"pass thiss!";
     }
   display.hit = HIT;
   //display.sendToGUI(winner_);




  // let the AI play
  //playAI();
  // update display communicate to GUI
 // display.sendToGUI(winner_);
}

void GorillaGame::playAI()
{
}
}
