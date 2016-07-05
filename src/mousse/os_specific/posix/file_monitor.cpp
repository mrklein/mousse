// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "file_monitor.hpp"
#include "iostreams.hpp"
#include "pstream.hpp"
#include "packed_list.hpp"
#include "pstream_reduce_ops.hpp"
#include "os_specific.hpp"
#include "reg_ioobject.hpp"     // for fileModificationSkew symbol
#ifdef MOUSSE_USE_INOTIFY
#   include <unistd.h>
#   include <sys/inotify.h>
#   include <sys/ioctl.h>
#   include <errno.h>
#   define EVENT_SIZE  ( sizeof (struct inotify_event) )
#   define EVENT_LEN   (EVENT_SIZE + 16)
#   define EVENT_BUF_LEN     ( 1024 * EVENT_LEN )
#else
#   include "os_specific.hpp"
#endif


// Static Data Members
const mousse::NamedEnum<mousse::fileMonitor::fileState, 3>
  mousse::fileMonitor::fileStateNames_;


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(fileMonitor, 0);

template<>
const char* mousse::NamedEnum
<
  mousse::fileMonitor::fileState,
  3
>::names[] =
{
  "unmodified",
  "modified",
  "deleted"
};

//- Reduction operator for PackedList of fileState
class reduceFileStates
{
  public:
  unsigned int operator()(const unsigned int x, const unsigned int y)
  const
  {
    // x,y are sets of 2bits representing fileState
    unsigned int mask = 3u;
    unsigned int shift = 0;
    unsigned int result = 0;
    while (mask)
    {
      // Combine state
      unsigned int xState = (x & mask) >> shift;
      unsigned int yState = (y & mask) >> shift;
      // Combine and add to result. Combine is such that UNMODIFIED
      // wins.
      unsigned int state = min(xState, yState);
      result |= (state << shift);
      shift += 2;
      mask <<= 2;
    }
    return result;
  }
};

//- Combine operator for PackedList of fileState
class combineReduceFileStates
{
  public:
  void operator()(unsigned int& x, const unsigned int y) const
  {
    x = reduceFileStates()(x, y);
  }
};

//-  Internal tracking via stat(3p) or inotify(7)
class fileMonitorWatcher
{
public:
  const bool useInotify_;
  // For inotify
    //- File descriptor for the inotify instance
    int inotifyFd_;
    //- Current watchIDs and corresponding directory id
    DynamicList<label> dirWatches_;
    DynamicList<fileName> dirFiles_;
  // For stat
    //- From watch descriptor to modified time
    DynamicList<time_t> lastMod_;
  //- Initialise inotify
  inline fileMonitorWatcher(const bool useInotify, const label sz = 20)
  :
    useInotify_{useInotify},
    inotifyFd_{-1}
  {
    if (useInotify_) {
#if defined(MOUSSE_USE_INOTIFY)
      inotifyFd_ = inotify_init();
      dirWatches_.setCapacity(sz);
      dirFiles_.setCapacity(sz);
      if (inotifyFd_ < 0) {
        static bool hasWarned = false;
        if (!hasWarned) {
          hasWarned = true;
          WARNING_IN("fileMonitorWatcher(const bool, const label)")
            << "Failed allocating an inotify descriptor : "
            << string(strerror(errno)) << endl
            << "    Please increase the number of allowable "
            << "inotify instances" << endl
            << "    (/proc/sys/fs/inotify/max_user_instances"
            << " on Linux)" << endl
            << "    , switch off runTimeModifiable." << endl
            << "    or compile this file without "
            << "MOUSSE_USE_INOTIFY"
            << " to use time stamps instead of inotify." << endl
            << "    Continuing without additional file"
            << " monitoring."
            << endl;
        }
      }
#else
      FATAL_ERROR_IN("fileMonitorWatcher(const bool, const label)")
        << "You selected inotify but this file was compiled"
        << " without MOUSSE_USE_INOTIFY"
        << " Please select another fileModification test method"
        << exit(FatalError);
#endif
    } else {
      lastMod_.setCapacity(sz);
    }
  }

  //- Remove all watches
  inline ~fileMonitorWatcher()
  {
#if defined(MOUSSE_USE_INOTIFY)
    if (useInotify_ && inotifyFd_ >= 0) {
      FOR_ALL(dirWatches_, i) {
        if (dirWatches_[i] >= 0) {
          if (inotify_rm_watch(inotifyFd_, int(dirWatches_[i]))) {
            WARNING_IN("fileMonitor::~fileMonitor()")
              << "Failed deleting directory watch "
              << dirWatches_[i] << endl;
          }
        }
      }
    }
#endif
  }

  inline bool addWatch(const label watchFd, const fileName& fName)
  {
    if (useInotify_) {
      if (inotifyFd_ < 0) {
        return false;
      }
#if defined(MOUSSE_USE_INOTIFY)
      // Add/retrieve watch on directory containing file.
      // Note that fName might be non-existing in special situations
      // (master-only reading for IODictionaries)
      const fileName dir = fName.path();
      label dirWatchID = -1;
      if (isDir(dir)) {
        dirWatchID = inotify_add_watch
        (
          inotifyFd_,
          dir.c_str(),
          IN_CLOSE_WRITE
        );
        if (dirWatchID < 0) {
          FATAL_ERROR_IN("addWatch(const label, const fileName&)")
            << "Failed adding watch " << watchFd
            << " to directory " << fName << " due to "
            << string(strerror(errno))
            << exit(FatalError);
        }
      }
      if (watchFd < dirWatches_.size() && dirWatches_[watchFd] != -1) {
        // Reuse of watchFd : should have dir watchID set to -1.
        FATAL_ERROR_IN("addWatch(const label, const fileName&)")
          << "Problem adding watch " << watchFd
          << " to file " << fName
          << abort(FatalError);
      }
      dirWatches_(watchFd) = dirWatchID;
      dirFiles_(watchFd) = fName.name();
#endif
    } else {
      if (watchFd < lastMod_.size() && lastMod_[watchFd] != 0) {
        // Reuse of watchFd : should have lastMod set to 0.
        FATAL_ERROR_IN("addWatch(const label, const fileName&)")
          << "Problem adding watch " << watchFd
          << " to file " << fName
          << abort(FatalError);
      }
      lastMod_(watchFd) = lastModified(fName);
    }
    return true;
  }


  inline bool removeWatch(const label watchFd)
  {
    if (useInotify_) {
      if (inotifyFd_ < 0) {
        return false;
      }
      dirWatches_[watchFd] = -1;
    } else {
      lastMod_[watchFd] = 0;
    }
    return true;
  }
};

}


// Private Member Functions
void mousse::fileMonitor::checkFiles() const
{
  if (useInotify_) {
#if defined(MOUSSE_USE_INOTIFY)
  // Large buffer for lots of events
    char buffer[EVENT_BUF_LEN];
    while (true) {
      struct timeval zeroTimeout = {0, 0};
      //- Pre-allocated structure containing file descriptors
      fd_set fdSet;
      // Add notify descriptor to select fd_set
      FD_ZERO(&fdSet);
      FD_SET(watcher_->inotifyFd_, &fdSet);
      int ready = select
      (
        watcher_->inotifyFd_+1,     // num filedescriptors in fdSet
        &fdSet,             // fdSet with only inotifyFd
        NULL,               // No writefds
        NULL,               // No errorfds
        &zeroTimeout        // eNo timeout
      );
      if (ready < 0) {
        FATAL_ERROR_IN("fileMonitor::checkFiles()")
          << "Problem in issuing select."
          << abort(FatalError);
      } else if (FD_ISSET(watcher_->inotifyFd_, &fdSet)) {
        // Read events
        ssize_t nBytes = ::read(watcher_->inotifyFd_, buffer, EVENT_BUF_LEN);
        if (nBytes < 0) {
          FATAL_ERROR_IN("fileMonitor::checkFiles()")
            << "read of " << watcher_->inotifyFd_
            << " failed with " << label(nBytes)
            << abort(FatalError);
        }
        // Go through buffer, consuming events
        int i = 0;
        while (i < nBytes) {
          const struct inotify_event* inotifyEvent =
            reinterpret_cast<const struct inotify_event*>(&buffer[i]);
          if ((inotifyEvent->mask & IN_CLOSE_WRITE) && inotifyEvent->len) {
            // Search for file
            FOR_ALL(watcher_->dirWatches_, i) {
              label id = watcher_->dirWatches_[i];
              if (id == inotifyEvent->wd
                  && inotifyEvent->name == watcher_->dirFiles_[i]) {
                // Correct directory and name
                localState_[i] = MODIFIED;
              }
            }
          }
          i += EVENT_SIZE + inotifyEvent->len;
        }
      } else {
        // No data
        return;
      }
    }
#endif
  } else {
    FOR_ALL(watcher_->lastMod_, watchFd) {
      time_t oldTime = watcher_->lastMod_[watchFd];
      if (oldTime != 0) {
        const fileName& fName = watchFile_[watchFd];
        time_t newTime = lastModified(fName);
        if (newTime == 0) {
          localState_[watchFd] = DELETED;
        } else {
          if (newTime > (oldTime + regIOobject::fileModificationSkew)) {
            localState_[watchFd] = MODIFIED;
          } else {
            localState_[watchFd] = UNMODIFIED;
          }
        }
      }
    }
  }
}


// Constructors
mousse::fileMonitor::fileMonitor(const bool useInotify)
:
  useInotify_{useInotify},
  localState_{20},
  state_{20},
  watchFile_{20},
  freeWatchFds_{2},
  watcher_{new fileMonitorWatcher{useInotify_, 20}}
{}


// Destructor
mousse::fileMonitor::~fileMonitor()
{}


// Member Functions
// Note: fName might not exist (on slaves if in master-only mode for
// regIOobject)
mousse::label mousse::fileMonitor::addWatch(const fileName& fName)
{
  label watchFd;
  label sz = freeWatchFds_.size();
  if (sz) {
    watchFd = freeWatchFds_[sz-1];
    freeWatchFds_.setSize(sz-1);
  } else {
    watchFd = state_.size();
  }
  watcher_->addWatch(watchFd, fName);
  if (debug) {
    Pout << "fileMonitor : added watch " << watchFd << " on file "
      << fName << endl;
  }
  if (watchFd < 0) {
    WARNING_IN("fileMonitor::addWatch(const fileName&)")
      << "could not add watch for file " << fName << endl;
  } else {
    localState_(watchFd) = UNMODIFIED;
    state_(watchFd) = UNMODIFIED;
    watchFile_(watchFd) = fName;
  }
  return watchFd;
}


bool mousse::fileMonitor::removeWatch(const label watchFd)
{
  if (debug) {
    Pout << "fileMonitor : removing watch " << watchFd << " on file "
      << watchFile_[watchFd] << endl;
  }
  freeWatchFds_.append(watchFd);
  return watcher_->removeWatch(watchFd);
}


const mousse::fileName& mousse::fileMonitor::getFile(const label watchFd) const
{
  return watchFile_[watchFd];
}


mousse::fileMonitor::fileState mousse::fileMonitor::getState(const label watchFd)
const
{
  return state_[watchFd];
}


void mousse::fileMonitor::updateStates
(
  const bool masterOnly,
  const bool syncPar
) const
{
  if (Pstream::master() || !masterOnly) {
    // Update the localState_
    checkFiles();
  }
  if (syncPar) {
    // Pack local state (might be on master only)
    PackedList<2> stats{state_.size(), MODIFIED};
    if (Pstream::master() || !masterOnly) {
      FOR_ALL(state_, watchFd) {
        stats[watchFd] = static_cast<unsigned int>(localState_[watchFd]);
      }
    }
    // Scatter or reduce to synchronise state
    if (masterOnly) {
      // Scatter
      if (stats.storage().size() == 1) {
        Pstream::scatter(stats.storage()[0]);
      } else {
        Pstream::listCombineScatter(stats.storage());
      }
    } else {
      // Reduce
      if (stats.storage().size() == 1) {
        // Optimisation valid for most cases.
        reduce(stats.storage()[0], reduceFileStates());
      } else {
        Pstream::listCombineGather
        (
          stats.storage(),
          combineReduceFileStates()
        );
      }
    }
    // Update synchronised state
    FOR_ALL(state_, watchFd) {
      // Assign synchronised state
      unsigned int stat = stats[watchFd];
      state_[watchFd] = fileState(stat);
      if (!masterOnly) {
        // Give warning for inconsistent state
        if (state_[watchFd] != localState_[watchFd]) {
          if (debug) {
            Pout << "fileMonitor : Delaying reading "
              << watchFile_[watchFd]
              << " due to inconsistent "
               "file time-stamps between processors"
              << endl;
          }
          WARNING_IN
          (
            "fileMonitor::updateStates"
            "(const bool, const bool) const"
          )
          << "Delaying reading " << watchFile_[watchFd]
          << " due to inconsistent file time-stamps between processors"
          << endl;
        }
      }
    }
  } else {
    state_ = localState_;
  }
}


void mousse::fileMonitor::setUnmodified(const label watchFd)
{
  state_[watchFd] = UNMODIFIED;
  localState_[watchFd] = UNMODIFIED;
  if (!useInotify_) {
    watcher_->lastMod_[watchFd] = lastModified(watchFile_[watchFd]);
  }
}

