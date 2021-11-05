#ifndef PCLOCK_HPP
#define PCLOCK_HPP

#include <iostream>

#include <pthread.h>
#include <chrono>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif // _WIN32

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;
//using namespace std;


//creating custom namespace in order to prevent 2 vars from having the same names
namespace pClock
{
  std::thread ClockThreadList[255]; //an array of threads (max 255)
  int LengthOfClockThreadList = 0;  //how many threads exist currently (max 255)
};

void sleepcp(int milliseconds) // Cross-platform sleep function directly and shamelessly stole from stack overflow
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif // _WIN32
}

template<class T>
int tick(T &t, int freq) //freq = how many ticks a second should have
{
  int ms = 1000/ freq;

  auto Tbegin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  bool out = t.run();
  auto Tend = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

  int tickLengthOfClockThreadListgth = std::max(0, ms - int(Tend - Tbegin)); //find out how long the tick took to execute

  sleepcp(tickLengthOfClockThreadListgth);

  if(out == true)
    tick(t, freq);

  return 1;
}

template<class T>
void pClockbegin(T &t, int ms)
{
  if(pClock::LengthOfClockThreadList < 255)
  {
    pClock::ClockThreadList[pClock::LengthOfClockThreadList] = std::thread([&t, ms](){tick(t, ms);});
    pClock::LengthOfClockThreadList++;
  }
  else
    std::cout << "Error: reached thread limit: can't create more than 255 threads" << std::endl;

}


void pClockend()
{
  for(int i = 0; i < pClock::LengthOfClockThreadList; i++)
    pClock::ClockThreadList[i].join();

  pClock::LengthOfClockThreadList = 0;
  memset(pClock::ClockThreadList, 0, sizeof(pClock::ClockThreadList));

}

#endif