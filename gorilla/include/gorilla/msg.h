#ifndef GORILLA_MSG_H
#define GORILLA_MSG_H
#include <fstream>
#include <thread>
#include <sys/stat.h>
namespace gorilla {
namespace {
static constexpr const char * fifo_name = "/tmp/duels_gorilla";
struct Timer
{
  std::chrono::steady_clock::time_point last;
  const std::chrono::milliseconds ms;
  inline Timer() : last(std::chrono::steady_clock::now()), ms(100) {}
  inline void sleep()
  {
    std::this_thread::sleep_until(last + ms);
    last = std::chrono::steady_clock::now();
  }
};
Timer timer;
}
inline void runGUI()
{
  remove(fifo_name);
  system("python3 ../gorilla_gui.py&");
  struct stat buffer;
  while(stat(fifo_name, &buffer))
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

inline void shutGUI()
{
    remove(fifo_name);
}

struct initMsg
{
  int x1; int y1; int x2; int y2; int yb[640]; int radius;//gorilla1+gorilla2 positions, building height, explosion radius
  void sendToGUI(std::string p1, std::string p2) const
  {
    std::ofstream fifo;
    fifo.open(fifo_name, std::ios::out);
    fifo << "init: " ;
    fifo << "\n- p1: " << p1;
    fifo << "\n- p2: " << p2;
    fifo << "\n- x1: " << x1;
    fifo << "\n- y1: " << y1;
    fifo << "\n- x2: " << x2;
    fifo << "\n- y2: " << y2;
    fifo << "\n- yb: " << "[";
    for(size_t i=0; i < 640; ++i)
      fifo << yb[i] << (i == 639?"]":", ");
    fifo << "\n- radius: " << radius;
    fifo << std::endl;
    fifo.close();
    timer.sleep();
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
  int x; int y; int hit;
  void sendToGUI(int winner = 0) const
  {
    std::ofstream fifo;
    fifo.open(fifo_name, std::ios::out);
    fifo << "winner: " << winner;
    fifo << "\nx: " << x;
    fifo << "\ny: " << y;
    fifo << "\nhit: " << hit;
    fifo << std::endl;
    fifo.close();
    timer.sleep();
    }
};

}
#endif
