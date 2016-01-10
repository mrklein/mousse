// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::JobInfo
// Description
//   Helper class for recording information about run/finished jobs.
//   Writes the following files:
//    - $FOAM_JOB_DIR/runningJobs
//    - $FOAM_JOB_DIR/finishedJobs
// SourceFiles
//   job_info.cpp
#ifndef job_info_hpp_
#define job_info_hpp_

#include "dictionary.hpp"
#include "file_name.hpp"
#include "cpu_time.hpp"

namespace mousse
{
class JobInfo
:
  public dictionary
{
  // Private data
    fileName runningJobPath_;
    fileName finishedJobPath_;
    cpuTime  cpuTime_;
  // Private Member Functions
    bool write(Ostream&) const;
    void end(const word& terminationType);
public:
  static bool constructed;
  static bool writeJobInfo;
  // Constructors
    //- Construct null
    JobInfo();
  //- Destructor
  ~JobInfo();
  // Member Functions
    // Write
      void write() const;
      void end();
      void exit();
      void abort();
      void signalEnd() const;
};
extern JobInfo jobInfo;
}  // namespace mousse
#endif
