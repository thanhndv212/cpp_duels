#include <example/msg.h>
#include <iostream>
#include <sys/stat.h>

using namespace example;

int main(int argc, char** argv)
{
  runGUI();

  feedbackMsg fb1_msg, fb2_msg;
  initMsg init_msg;
  inputMsg input;
  displayMsg display;

  // prepare init message
  init_msg.x = rand() % 10;
  init_msg.y = rand() % 10;
  init_msg.r = 0.5;
  for(int i = 0; i < 10; ++i)
    init_msg.d[i] = i*i;

  auto last = std::chrono::steady_clock::now();
  init_msg.sendToGUI("player 1", "player 2");
  display.sendToGUI();

  int winner(0);

  while(winner == 0)
  {

    auto now = std::chrono::steady_clock::now();
    std::cout << "Loop duration: " <<
                 std::chrono::duration_cast<std::chrono::milliseconds>(now-last).count() << " ms " << std::endl;
    last = now;


    // update game state



    // update display
    display.x = rand() % 10;
    display.y = rand() % 10;
    display.hit = false;

    // random winner in this example
    const int dice(rand() % 100);

    if(dice > 95)
      winner = 1;
    else if(dice > 97)
      winner = 2;

    std::cout << "sending x = " << display.x << ", y = " << display.y
                  << ", winner = " << winner << std::endl;

    display.sendToGUI(winner);
    if(winner)
      break;
  }
  std::cout << "Player " << winner << " has won!\n";
}
