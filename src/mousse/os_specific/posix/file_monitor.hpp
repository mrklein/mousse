#ifndef OS_SPECIFIC_POSIX_FILE_MONITOR_HPP_
#define OS_SPECIFIC_POSIX_FILE_MONITOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileMonitor
// Description
//   Checking for changes to files.
// Note
//   The default is to use stat to get the timestamp.
//   Compile with MOUSSE_USE_INOTIFY to use the inotify
//   (Linux specific, since 2.6.13) framework. The problem is that inotify does
//   not work on nfs3 mounted directories!!

#include <sys/types.h>
#include "named_enum.hpp"
#include "class_name.hpp"
#include "dynamic_list.hpp"


namespace mousse {

class fileMonitor;
class fileMonitorWatcher;


class fileMonitor
{
public:
  // Public data types
    //- Enumeration defining the file state.
    enum fileState
    {
      UNMODIFIED = 0,
      MODIFIED = 1,
      DELETED = 2
    };
    static const NamedEnum<fileState, 3> fileStateNames_;
private:
  // Private data
    //- Whether to use inotify (requires -DMOUSSE_USE_INOTIFY, see above)
    const bool useInotify_;
    //- State for all watchFds based on local files
    mutable DynamicList<fileState> localState_;
    //- State for all watchFds - synchronised
    mutable DynamicList<fileState> state_;
    //- Filename for all watchFds
    DynamicList<fileName> watchFile_;
    //- Free watchFds
    DynamicList<label> freeWatchFds_;
    //- Watch mechanism (stat or inotify)
    mutable autoPtr<fileMonitorWatcher> watcher_;
  // Private Member Functions
    //- Update localState_ from any events.
    void checkFiles() const;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("fileMonitor");
  // Constructors
    //- Construct null
    fileMonitor(const bool useInotify);
    //- Disallow default bitwise copy construct
    fileMonitor(const fileMonitor&) = delete;
    //- Disallow default bitwise assignment
    fileMonitor& operator=(const fileMonitor&) = delete;
  //- Destructor
  ~fileMonitor();
  // Member Functions
    //- Add file to watch. Return watch descriptor
    label addWatch(const fileName&);
    //- Remove file to watch. Return true if successful
    bool removeWatch(const label watchFd);
    //- Get name of file being watched
    const fileName& getFile(const label watchFd) const;
    //- Check state using handle
    fileState getState(const label watchFd) const;
    //- Check state of all files. Updates state_.
    void updateStates
    (
      const bool masterOnly,
      const bool syncPar
    ) const;
    //- Reset state (e.g. after having read it) using handle
    void setUnmodified(const label watchFd);
};
}  // namespace mousse
#endif
