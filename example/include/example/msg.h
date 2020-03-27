#ifndef EXAMPLE_MSG_H
#define EXAMPLE_MSG_H
#include <fstream>
#include <thread>
#include <sys/stat.h>
namespace example {
static constexpr const char * fifo_name = "/tmp/duels_example";
void runGUI()
{
  remove(fifo_name);
  system("python3 ../example_gui.py&");
  struct stat buffer;
  while(stat(fifo_name, &buffer))
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
struct initMsg
{
  int x; int y; double r; double d[10];
  void sendToGUI(std::string p1, std::string p2) const
  {
    std::ofstream fifo;
    fifo.open(fifo_name, std::ios::out);
    fifo << "p1: " << p1;
    fifo << "\np2: " << p2;
    fifo << "\nx: " << x;
    fifo << "\ny: " << y;
    fifo << "\nr: " << r;
    fifo << "\nd: " << "[";
    for(size_t i=0; i < 10; ++i)
      fifo << d[i] << (i == 9?"]":", ");
    fifo << std::endl;
    fifo.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
};

struct inputMsg
{
  double angle; double force;
};

struct feedbackMsg
{
  int x; int y; int xo; int yo; int xb; int yb; int wind;
  feedbackMsg() {}
  feedbackMsg(int _x, int _y, int _xo, int _yo, int _xb, int _yb, int _wind)
    : x(_x), y(_y), xo(_xo), yo(_yo), xb(_xb), yb(_yb), wind(_wind) {}
};

struct displayMsg
{
  double x; double y; bool hit;
  void sendToGUI(int winner = 0) const
  {
    std::ofstream fifo;
    fifo.open("/tmp/duels_example", std::ios::out);
    fifo << "winner: " << winner;
    fifo << "\nx: " << x;
    fifo << "\ny: " << y;
    fifo << "\nhit: " << hit;
    fifo << std::endl;
    fifo.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
};

}
#endif
