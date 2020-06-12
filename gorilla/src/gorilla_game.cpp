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
  inputMsg input;
  feedbackMsg fb1_msg, fb2_msg;
  displayMsg display;

  // prepare init message
  initMsg init_msg;
  init_msg = gameSet();
  init_msg.sendToGUI("player 1", "player 2");
  display.sendToGUI();


  //prepare input and display input message
  int winner = 0;
  int i = 0;
  int first_turn =1;
  int turn = first_turn;

  while(winner == 0)
  {
      input = getInput();
      play(init_msg, input, display, turn);
      if(turn==1)
      {
          turn = 2;
      }
      else
          turn = 1;
      i++;
      if(i== 3)
      {
         cout<<"no one has won!!\n";
         break;
       }
      if(winner)
      {
          cout<<"player "<<winner<<" has won!\n";
          break;
       }

}
}
bool inplay_check(int x)
{
    if (x <0 || x > 639)
        return false;
        cout<<"xPos of banana "<<x<<'\n';
    return true;

}

int hit_check(initMsg initMsg, int x, int y)
{
    int bottomLine = 335;
    int Gor1_xPos = floor(initMsg.x1/64)*64 +32;
    int Gor1_yPos = initMsg.y1 -15 ;
    int Gor2_xPos = floor(initMsg.x2/64)*64 +32;
    int Gor2_yPos = initMsg.y2 -15;
    int epl_rad = initMsg.radius ;
    double Gor1_check = (sqrt((x-Gor1_xPos)*(x-Gor1_xPos)+(y-Gor1_yPos)*(y-Gor1_yPos)));
    cout<<x<<" ; "<<Gor1_xPos<<" ; "<<y<<" ; "<<Gor1_yPos<<"\n";
    cout<<x<<" ; "<<Gor2_xPos<<" ; "<<y<<" ; "<<Gor2_yPos<<"\n";
    cout<<"distance from banana to gor1: "<<Gor1_check<<"\n";
    double Gor2_check = (sqrt((x-Gor2_xPos)*(x-Gor2_xPos)+(y-Gor2_yPos)*(y-Gor2_yPos)));
    cout<<"distance from banana to gor2: "<<Gor2_check<<"\n";
    //int Bd_check = int(sqrt((x-0)*(x-0)+(y-initMsg.yb[0])*(y-initMsg.yb[0])));


    /*for (int i = 0; i < 640; ++i) {
       int new_Bd_check = int(sqrt((x-i)*(x-i)+(y-initMsg.yb[i])*(y-initMsg.yb[i])));
       //if (Bd_check > new_Bd_check)
           //Bd_check = new_Bd_check;
    }
    */
    if(Gor1_check<=epl_rad)
        return 1;//banana hit gorilla 1
    if(Gor2_check<=epl_rad)
        return 2;//banana hit gorilla 2
    if(y >= bottomLine - initMsg.yb[x]- 6)
        return 3;
    //if(new_Bd_check<=epl_rad)
        //return 3;//banana hit buildings
    return 0;
}

initMsg GorillaGame::gameSet(){
    initMsg init_msg;
    int bottomLine = 335;
    init_msg.x1 = rand()%100 + 100;
    init_msg.x2 = rand()%300 + 220;
    init_msg.radius = 10;
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
  const double gravity = 12;
  //velocity and angle
  double angle = input.angle;
  double vel = input.force;
  double cosine = 0;
  double sine = 0;
  feedbackMsg msg;
  if(turn ==1 ){
      msg.x = initMsg.x1;
      msg.y = initMsg.y1;
      msg.xo = initMsg.x2;
      msg.yo = initMsg.y2;
      msg.wind = rand()%2 - 2;
      cosine = cos(angle*PI/180);
      sine = sin(angle*PI/180);
  }
  if(turn==2)
  {
      msg.x = initMsg.x2;
      msg.y = initMsg.y2;
      msg.xo = initMsg.x1;
      msg.yo = initMsg.y1;
      msg.wind = rand()%2 - 2;
      angle = 180 - angle;
      cosine = cos(angle*PI/180);
      sine = sin(angle*PI/180);
      //cosine = -cos(angle*PI/180);
      //sine = sin(angle*PI/180);
  }
  //starting position of banana
  int start_x = msg.x;
  int start_y = msg.y;


  int wind = msg.wind;

  int banana_x;
  int banana_y;
  //handling time
  double t = traveling_time;
  //update banana's position
  banana_x = int(start_x + vel*cosine*t + (1/2)*wind*t*t);
  banana_y = int(start_y - vel*sine*t + gravity*t*t);
  //banana_x = int(start_x + vel*cosine*t + (1/2)*wind*t*t);
  //banana_y = int(start_y + vel*sine*t - (1/2)*gravity*t*t);

  msg.xb = banana_x;
  msg.yb = banana_y;
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
   int hit_;

   while (not HIT && inplay)
     {
         fb_msg = getFeedback(initMsg, input, turn, t);
         banana_x = fb_msg.xb;
         banana_y = fb_msg.yb;
         inplay = inplay_check(banana_x);
         if(!inplay)
         {
             cout<<"Your shot went out of range!\n";
             break;
         }

         display.x = banana_x;
         display.y = banana_y;
         hit_ = hit_check(initMsg,banana_x,banana_y);
         //check if hit targeted gorilla --> HIT = 1; winner = turn;
         //if(HIT){over();winner()}
         if(hit_ == 0)
         {
             HIT = 0;
             display.hit = hit_;
             display.sendToGUI();
             t+= 0.15;
         }


         if(hit_ == 1)
         {
             HIT = 1;
             display.hit = HIT;
             winner_ = 2;
             display.sendToGUI(winner_);
             cout<<"Gorilla 1 was hit at ("<<banana_x<<","<<banana_y<<")\n";
             cout<<"Gorilla 2 won!!!!!!!!!!!\n";

             break;
         }
         if(hit_ == 2)
         {
             HIT = 1;
             display.hit = HIT;
             winner_ = 1;
             display.sendToGUI(winner_);
             cout<<"Gorilla 2 was hit at ("<<banana_x<<","<<banana_y<<")\n";
             cout<<"Gorilla 1 won!!!!!!!!!!!\n";
             break;
         }
         if(hit_ == 3)
         {
             HIT = 3;
             display.hit = hit_;
             display.sendToGUI();
             cout<<"BUILDINGS was hit at ("<<banana_x<<","<<banana_y<<")\n";
             //build hit signal need to be sent////////////////////////////////////
             break;
         }
         //display.sendToGUI();
         //t+= 0.2;


     }
   //display.hit = HIT;





  // let the AI play
  //playAI();
  // update display communicate to GUI
 // display.sendToGUI(winner_);
}

void GorillaGame::playAI()
{
}
}
