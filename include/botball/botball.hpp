#ifndef _BOTBALL_HPP_
#define _BOTBALL_HPP_

#include <wallaby/thread.hpp>

class ShutDownIn : public Thread
{
public:
  ShutDownIn(double s);
  
  virtual void run();
  
private:
  double m_s;
};

#endif