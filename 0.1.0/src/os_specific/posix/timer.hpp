#ifndef OS_SPECIFIC_POSIX_TIMER_HPP_
#define OS_SPECIFIC_POSIX_TIMER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timer
// Description
//   Implements a timeout mechanism via sigalarm.
//   Example usage:
//   \code
//     timer myTimer(5);     // 5 sec
//     ..
//     if (timedOut(myTimer))
//     {
//       // timed out
//     }
//     else
//     {
//       // do something possible blocking
//     }
//   \endcode
//   Constructor set signal handler on sigalarm and alarm(). Destructor
//   clears these.
//   timedOut is macro because setjmp can't be in member function of timer.
//   ?something to do with stack frames.
// Warning
//   The setjmp restores complete register state so including local vars
//   held in regs. So if in blocking part something gets calced in a stack
//   based variable make sure it is declared 'volatile'.

#include "class_name.hpp"
#include <signal.h>
#include <setjmp.h>


//- Check it a timeout has occured
//  keep setjmp in same stack frame so no function calls
#define timedOut(x) \
  (((x).newTimeOut_ > 0) ? setjmp(mousse::timer::envAlarm) : false)

namespace mousse {

class timer
{
  // Private data
    //- Old signal masks
    static struct sigaction oldAction_;
    //- Old alarm() value
    static unsigned int oldTimeOut_;
  // Private Member Functions
    //- Alarm handler
    static void signalHandler(int);
public:
  // Public data
    //- Declare name of the class and its debug switch
    CLASS_NAME("timer");
    //- Current time out value. Needed by macro timedOut
    unsigned int newTimeOut_;
    //- State for setjmp. Needed by macro timedOut
    static jmp_buf envAlarm;
  // Constructors
    //- Construct from components.
    //  newTimeOut=0 makes it do nothing.
    timer(const unsigned int newTimeOut);
  //- Destructor
  ~timer();
};

}  // namespace mousse
#endif
