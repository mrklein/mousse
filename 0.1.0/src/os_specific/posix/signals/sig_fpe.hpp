#ifndef OS_SPECIFIC_POSIX_SIGNALS_SIG_FPE_HPP_
#define OS_SPECIFIC_POSIX_SIGNALS_SIG_FPE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sigFpe
// Description
//   Set up trapping for floating point exceptions (signal FPE).
//   Controlled by two env vars:
//   \param MOUSSE_SIGFPE \n
//     exception trapping
//   \param MOUSSE_SETNAN \n
//     initialization of all malloced memory to NaN. If MOUSSE_SIGFPE
//     also set, this will cause usage of uninitialized scalars to trigger
//     an abort.
// SourceFiles
//   sig_fpe.cpp
#include <signal.h>
#if defined(linux) || defined(linux64) || defined(linuxIA64) || \
  defined(linuxARM7) || defined(linuxPPC64) || defined(linuxPPC64le)
  #define LINUX
#endif
#if defined(LINUX) && defined(__GNUC__)
  #define LINUX_GNUC
#endif
#if defined(__APPLE__)
#include <malloc/malloc.h>
#endif
#include "ulist.hpp"
namespace mousse
{
class sigFpe
{
  // Private data
    //- Saved old signal trapping setting
    static struct sigaction oldAction_;
#if defined(darwin64)
    static void *(*oldMalloc_)(struct _malloc_zone_t *zone, size_t size);
    static void *nanMalloc_(struct _malloc_zone_t *zone, size_t size);
#endif
  // Static data members
    #if defined(LINUX_GNUC) || defined(darwin64)
    //- Handler for caught signals
    static void sigHandler(int);
    #endif
public:
  // Constructors
    //- Construct null
    sigFpe();
  //- Destructor
  ~sigFpe();
  // Member functions
    //- Activate SIGFPE signal handler when MOUSSE_SIGFPE is %set
    //  Fill memory with NaN when MOUSSE_SETNAN is %set
    void set(const bool verbose);
    //- Flag to indicate mallocNan is enabled
    static bool mallocNanActive_;
    #if defined(LINUX)
    //- Malloc function which initializes to NaN
    static void* mallocNan(size_t size);
    #endif
    //- Fill block of data with NaN
    static void fillNan(UList<scalar>&);
};
}  // namespace mousse
#endif
