#ifndef CLOGGER_HPP
#define CLOGGER_HPP

class clogger
{
public:

  bool run()
  {
    for(int i = 0; i < 20; ++i)
    {
        std::cout << i << std::endl;
    }
    return false;
  }
};

#endif