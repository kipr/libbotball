#include "botball.hpp"

ShutDownIn::ShutDownIn(double s)
  : m_s(s)
{
  
}
virtual void ShutDownIn::run()
{
  const double start = seconds();
  msleep(m_s * 1000.0);
  const double end = seconds();
  std::cout << std::endl << "Shutdown after " << (end - start) << " seconds" << std::endl;
  
	// TODO: Might want to move this to botui in the future.
	Create::instance()->stop();
	_exit(0);
}