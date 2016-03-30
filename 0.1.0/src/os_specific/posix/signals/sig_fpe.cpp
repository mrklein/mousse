// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_fpe.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "os_specific.hpp"
#include "iostreams.hpp"
#ifdef LINUX_GNUC
  #ifndef __USE_GNU
    #define __USE_GNU
  #endif
  #include <fenv.h>
  #include <malloc.h>
#elif defined(sgiN32) || defined(sgiN32Gcc)
  #include <sigfpe.h>
#endif
#if defined(darwin64)
#include <xmmintrin.h>
#include <malloc/malloc.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
#include <limits>


// Static Data Members
struct sigaction mousse::sigFpe::oldAction_;

void mousse::sigFpe::fillNan(UList<scalar>& lst)
{
  lst = std::numeric_limits<scalar>::signaling_NaN();
}

bool mousse::sigFpe::mallocNanActive_ = false;

#if defined(darwin64)
void *(*mousse::sigFpe::oldMalloc_)(struct _malloc_zone_t *zone, size_t size)
= NULL;
void *mousse::sigFpe::nanMalloc_(struct _malloc_zone_t *zone, size_t size)
{
  void *result;
  result = oldMalloc_(zone, size);
  if (mallocNanActive_) {
    UList<scalar> lst
    (
      reinterpret_cast<scalar*>(result),
      size/sizeof(scalar)
    );
    fillNan(lst);
  }
  return result;
}
#endif

#ifdef LINUX
extern "C"
{
  extern void* __libc_malloc(size_t size);
  // Override the GLIBC malloc to support mallocNan
  void* malloc(size_t size)
  {
    if (mousse::sigFpe::mallocNanActive_) {
      return mousse::sigFpe::mallocNan(size);
    } else {
      return __libc_malloc(size);
    }
  }
}


void* mousse::sigFpe::mallocNan(size_t size)
{
  // Call the low-level GLIBC malloc function
  void * result = __libc_malloc(size);
  // Initialize to signalling NaN
  UList<scalar> lst{reinterpret_cast<scalar*>(result), size/sizeof(scalar)};
  sigFpe::fillNan(lst);
  return result;
}
#endif


#if defined(LINUX_GNUC) || defined(darwin64)
void mousse::sigFpe::sigHandler(int)
{
  // Reset old handling
  if (sigaction(SIGFPE, &oldAction_, NULL) < 0) {
    FATAL_ERROR_IN
    (
      "mousse::sigSegv::sigHandler()"
    )
    << "Cannot reset SIGFPE trapping"
    << abort(FatalError);
  }
  // Update jobInfo file
  jobInfo.signalEnd();
  error::printStack(Perr);
  // Throw signal (to old handler)
  raise(SIGFPE);
}
#endif


// Constructors 
mousse::sigFpe::sigFpe()
{
  oldAction_.sa_handler = NULL;
}


// Destructor 
mousse::sigFpe::~sigFpe()
{
  if (env("MOUSSE_SIGFPE")) {
#if defined(LINUX_GNUC) || defined(darwin64)
    // Reset signal
#if defined(darwin64)
    if (oldAction_.sa_handler && sigaction(SIGFPE, NULL, NULL) < 0) {
#else
    if (oldAction_.sa_handler && sigaction(SIGFPE, &oldAction_, NULL) < 0) {
#endif
      FATAL_ERROR_IN
      (
        "mousse::sigFpe::~sigFpe()"
      )
      << "Cannot reset SIGFPE trapping"
      << abort(FatalError);
    }
#endif
  }
  if (env("MOUSSE_SETNAN")) {
#if defined(LINUX) || defined(darwin64)
    // Disable initialization to NaN
    mallocNanActive_ = false;
#endif
#if defined(darwin64)
    // Restoring old malloc handler
    if (oldMalloc_ != NULL) {
      malloc_zone_t *zone = malloc_default_zone();
      if (zone != NULL) {
        mprotect(zone, getpagesize(), PROT_READ | PROT_WRITE);
        zone->malloc = oldMalloc_;
        mprotect(zone, getpagesize(), PROT_READ);
      }
    }
#endif
  }
}


// Member Functions 
void mousse::sigFpe::set(const bool verbose)
{
  if (oldAction_.sa_handler) {
    FATAL_ERROR_IN
    (
      "mousse::sigFpe::set()"
    )
    << "Cannot call sigFpe::set() more than once"
    << abort(FatalError);
  }
  if (env("MOUSSE_SIGFPE")) {
    bool supported = false;
#if defined(LINUX_GNUC) || defined(darwin64)
    supported = true;
#if defined(LINUX_GNUC)
    feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif  // LINUX_GNUC
#if defined(darwin64)
    _mm_setcsr(_MM_MASK_MASK &~
         (_MM_MASK_OVERFLOW|_MM_MASK_INVALID|_MM_MASK_DIV_ZERO));
#endif  // darwin64
    struct sigaction newAction;
    newAction.sa_handler = sigHandler;
    newAction.sa_flags = SA_NODEFER;
    sigemptyset(&newAction.sa_mask);
#if defined(darwin64)
    if (sigaction(SIGFPE, &newAction, NULL) < 0) {
#else
    if (sigaction(SIGFPE, &newAction, &oldAction_) < 0) {
#endif
      FATAL_ERROR_IN
      (
        "mousse::sigFpe::set()"
      )
      << "Cannot set SIGFPE trapping"
      << abort(FatalError);
    }
#elif defined(sgiN32) || defined(sgiN32Gcc)
    supported = true;
    sigfpe_[_DIVZERO].abort=1;
    sigfpe_[_OVERFL].abort=1;
    sigfpe_[_INVALID].abort=1;
    sigfpe_[_DIVZERO].trace=1;
    sigfpe_[_OVERFL].trace=1;
    sigfpe_[_INVALID].trace=1;
    handle_sigfpes
    (
      _ON,
      _EN_DIVZERO | _EN_INVALID | _EN_OVERFL,
      0,
      _ABORT_ON_ERROR,
      NULL
    );
#endif
    if (verbose) {
      if (supported) {
        Info << "sigFpe : Enabling floating point exception trapping"
          << " (MOUSSE_SIGFPE)." << endl;
      } else {
        Info << "sigFpe : Floating point exception trapping"
          << " - not supported on this platform" << endl;
      }
    }
  }
  if (env("MOUSSE_SETNAN")) {
#if defined(LINUX)
    mallocNanActive_ = true;
#endif
#if defined(darwin64)
    malloc_zone_t *zone = malloc_default_zone();
    if (zone != NULL) {
      oldMalloc_ = zone->malloc;
      if (mprotect(zone, getpagesize(), PROT_READ | PROT_WRITE) == 0) {
        zone->malloc = nanMalloc_;
        if (mprotect(zone, getpagesize(), PROT_READ) == 0) {
          mallocNanActive_ = true;
        }
      }
    }
#endif
    if (verbose) {
      if (mallocNanActive_) {
        Info << "SetNaN : Initialising allocated memory to NaN"
          << " (MOUSSE_SETNAN)." << endl;
      } else {
        Info << "SetNaN : Initialise allocated memory to NaN"
          << " - not supported on this platform" << endl;
      }
    }
  }
}

