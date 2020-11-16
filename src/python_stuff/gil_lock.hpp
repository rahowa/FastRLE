#include <boost/python.hpp>

#ifndef GIL_LOCK_HEADER
#define GIL_LOCK_HEADER

class GilLock{
public:
  GilLock(): state_(PyGILState_Ensure()){}
  ~GilLock() { PyGILState_Release(state_);   }
private:
  PyGILState_STATE state_;
};


#endif