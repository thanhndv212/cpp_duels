#include <gorilla/gorilla_game.h>
#include <gorilla/msg.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
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
      winner = winner_ ;
      if(turn==1)
      {
          turn = 2;
      }
      else
          turn = 1;
      i++;
      if(i == 3) //maximum of turns to be played by 2 players before draw
      {
         cout<<"no one has won!!\n";
         winner = 3;
         display.sendToGUI(winner);
         break;
       }
      if(winner != 0)
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
    return true;

}

int hit_check(initMsg initMsg, int x, int y)
{
    int bottomLine = 335;
    int Gor1_xPos = int(initMsg.x1/64)*64 +32;
    int Gor1_yPos = initMsg.y1 -15 ;
    int Gor2_xPos = int(initMsg.x2/64)*64 +32;
    int Gor2_yPos = initMsg.y2 -15;
    int epl_rad = initMsg.radius ;

    //cout<<x<<" ; "<<Gor1_xPos<<" ; "<<y<<" ; "<<Gor1_yPos<<"\n";
    //cout<<x<<" ; "<<Gor2_xPos<<" ; "<<y<<" ; "<<Gor2_yPos<<"\n";

    double Gor1_check = (sqrt((x-Gor1_xPos)*(x-Gor1_xPos)+(y-Gor1_yPos)*(y-Gor1_yPos)));
    cout<<"distance from banana to gor1: "<<Gor1_check<<"\n";

    double Gor2_check = (sqrt((x-Gor2_xPos)*(x-Gor2_xPos)+(y-Gor2_yPos)*(y-Gor2_yPos)));
    cout<<"distance from banana to gor2: "<<Gor2_check<<"\n";

    double Bd_check = (sqrt((x-0)*(x-0)+(y-initMsg.yb[0])*(y-initMsg.yb[0])));
    for (int i = 0; i < 640; ++i)
    {
       double new_Bd_check = (sqrt((x-i)*(x-i)+(y-initMsg.yb[i])*(y-initMsg.yb[i])));
       if (Bd_check > new_Bd_check)
           Bd_check = new_Bd_check;
    }
    cout<<"Bd_check is: "<<Bd_check<<'\n';
    if(Gor1_check<=epl_rad)
        return 1;//banana hit gorilla 1
    if(Gor2_check<=epl_rad)
        return 2;//banana hit gorilla 2
    //if(Bd_check <= epl_rad)
    //    return 3;
    if(y>=bottomLine-initMsg.yb[x]-6)
    {
        cout<<"distance to building: "<<(-y+bottomLine-initMsg.yb[x])<<'\n';
        return 3;//banana hit buildings
    }
    cout<<"distance to building: "<<(-y+bottomLine-initMsg.yb[x])<<'\n';
    return 0;
}

initMsg GorillaGame::gameSet(){
    initMsg init_msg;
    int bottomLine = 335;
    srand((unsigned) time(0));
    init_msg.x1 = (rand()%319)  ;
    init_msg.x2 = (rand()%319) + 320;
    init_msg.radius = 12;
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
    cout<<"Enter a angle: ";
    while(!(cin>>input.angle))
    {
          cin.clear();
          cin.ignore(1000,'\n');
          cout<<"Invalid value. Try numeric value only!\n";\
          cout<<"Enter a angle: ";
    }
    cout<<"You have entered: "<<input.angle<<'\n';
    cout<<"Enter a velocity: ";
    while(!(cin>>input.force))
    {
          cin.clear();
          cin.ignore(1000,'\n');
          cout<<"Invalid value. Try numeric value only!\n";\
          cout<<"Enter a velocity: ";
    }
    cout<<"You have entered: "<<input.force<<'\n';

    return input;
}

feedbackMsg GorillaGame::getFeedback(initMsg initMsg, inputMsg input, int turn, double traveling_time)
{
  // whatever data we need to communicate to the player
  feedbackMsg msg;
  const double gravity = 10;
  //velocity and angle
  double angle = input.angle;
  double vel = input.force;
  double cosine = 0;
  double sine = 0;

  if(turn ==1 ){
      msg.x = int(initMsg.x1/64)*64 + 32 + initMsg.radius;
      msg.y = initMsg.y1 - 15 - initMsg.radius;
      msg.xo = int(initMsg.x2/64)*64 + 32 ;
      msg.yo = initMsg.y2 - 15;
      msg.wind = rand()%4 - 2;
      cosine = cos(angle*PI/180);
      sine = sin(angle*PI/180);
  }
  if(turn==2)
  {
      msg.x = int(initMsg.x2/64)*64 + 32 - initMsg.radius ;
      msg.y = initMsg.y2 - 15 - initMsg.radius;
      msg.xo = int(initMsg.x1/64)*64 + 32;
      msg.yo = initMsg.y1 - 15;
      msg.wind = rand()%4 - 2;
      angle = 180 - angle;
      cosine = cos(angle*PI/180);
      sine = sin(angle*PI/180);
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
  if(int(vel-0) == 0)
  {
     cout<<"velocity is zero!!!\n";
     banana_x = int(start_x/64)*64 + 32;
     banana_y = start_y - 15;
  }
  else
  {
     banana_x = int(start_x + vel*cosine*t + 0.01*wind*t*t);
     banana_y = int(start_y - vel*sine*t + 0.5*gravity*t*t);
  }

  msg.xb = banana_x;
  msg.yb = banana_y;
  return msg;
}

void GorillaGame::play(initMsg initMsg, inputMsg input, displayMsg display, int turn)
{

  cout<<"This is player"<<turn<<"'s turn!\n";

  cout<<"Player has shoot a banana with a velocity of "<<input.force<<" at a angle of "<<input.angle<<'\n';

  double t = 0.1;
  feedbackMsg fb_msg;

  int banana_x;
  int banana_y;
  bool HIT = 0;//whether hit gor
  int bottomline = 335;
  bool inplay = 1;
  int hit_;//hit what

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
         int xtoBuildEdge, ytoBuildEdge;
         if(hit_ == 3)
         {
             HIT = 3;
             ytoBuildEdge = banana_y - (bottomline - initMsg.yb[banana_x]) ;
             if (turn == 1)
             {
                 xtoBuildEdge = (banana_x + 1) % 64 - 1;
                 if (xtoBuildEdge < ytoBuildEdge)
                 {
                     display.x = banana_x - 0.8*xtoBuildEdge;
                     display.y = banana_y - 0.8*xtoBuildEdge;
                 }
                 else {
                     display.x = banana_x - 0.8*ytoBuildEdge;
                     display.y = banana_y - 0.8*ytoBuildEdge;
                 }
             }
             if (turn == 2)
             {
                 xtoBuildEdge = 63 - ((banana_x + 1) % 64 - 1 );
                 if (xtoBuildEdge < ytoBuildEdge)
                 {
                     display.x = banana_x + 0.8*xtoBuildEdge;
                     display.y = banana_y - 0.8*xtoBuildEdge;
                 }
                 else {
                     display.x = banana_x + 0.8*ytoBuildEdge;
                     display.y = banana_y - 0.8*ytoBuildEdge;
                 }
             }
             display.hit = hit_;
             display.sendToGUI();
             cout<<"BUILDINGS was hit at ("<<display.x<<","<<display.y<<")\n";
             break;
         }

     }
   cout<<"------------------------------------\n";


  // let the AI play
  //playAI();
  // update display communicate to GUI
 // display.sendToGUI(winner_);
}

void GorillaGame::playAI()
{}

}
