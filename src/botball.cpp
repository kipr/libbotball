#include "botball/botball.hpp"

#include <wallaby/create.hpp>
#include <wallaby/util.h>
#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

ShutDownIn::ShutDownIn(double s)
  : m_s(s)
{
  
}
void ShutDownIn::run()
{
  const double start = seconds();
  msleep(m_s * 1000.0);
  const double end = seconds();
  std::cout << std::endl << "Shutdown after " << (end - start) << " seconds" << std::endl;
  
	// TODO: Might want to move this to botui in the future.
	Create::instance()->stop();
	_exit(0);
}