// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "job_info.hpp"
#include "os_specific.hpp"
#include "clock.hpp"
#include "ofstream.hpp"
#include "pstream.hpp"
// Static Data Members
bool mousse::JobInfo::writeJobInfo(mousse::debug::infoSwitch("writeJobInfo", 0));
mousse::JobInfo mousse::jobInfo;
// Constructors 
// Null constructor
mousse::JobInfo::JobInfo()
:
  runningJobPath_(),
  finishedJobPath_(),
  cpuTime_()
{
  name() = "JobInfo";
  if (writeJobInfo && Pstream::master())
  {
    string baseDir = getEnv("MOUSSE_JOB_DIR");
    string jobFile = hostName() + '.' + mousse::name(pid());
    fileName runningDir(baseDir/"running_jobs");
    fileName finishedDir(baseDir/"finished_jobs");
    runningJobPath_  = runningDir/jobFile;
    finishedJobPath_ = finishedDir/jobFile;
    if (baseDir.empty())
    {
      FATAL_ERROR_IN("JobInfo::JobInfo()")
        << "Cannot get JobInfo directory $MOUSSE_JOB_DIR"
        << mousse::exit(FatalError);
    }
    if (!isDir(runningDir) && !mkDir(runningDir))
    {
      FATAL_ERROR_IN("JobInfo::JobInfo()")
        << "Cannot make JobInfo directory " << runningDir
        << mousse::exit(FatalError);
    }
    if (!isDir(finishedDir) && !mkDir(finishedDir))
    {
      FATAL_ERROR_IN("JobInfo::JobInfo()")
        << "Cannot make JobInfo directory " << finishedDir
        << mousse::exit(FatalError);
    }
  }
  constructed = true;
}
// Destructor 
mousse::JobInfo::~JobInfo()
{
  if (writeJobInfo && constructed && Pstream::master())
  {
    mv(runningJobPath_, finishedJobPath_);
  }
  constructed = false;
}
// Member Functions 
bool mousse::JobInfo::write(Ostream& os) const
{
  if (writeJobInfo && Pstream::master())
  {
    if (os.good())
    {
      dictionary::write(os, false);
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return true;
  }
}
void mousse::JobInfo::write() const
{
  if (writeJobInfo && Pstream::master())
  {
    if (!write(OFstream(runningJobPath_)()))
    {
      FATAL_ERROR_IN("JobInfo::write() const")
        << "Failed to write to JobInfo file "
        << runningJobPath_
        << mousse::exit(FatalError);
    }
  }
}
void mousse::JobInfo::end(const word& terminationType)
{
  if (writeJobInfo && constructed && Pstream::master())
  {
    add("cpuTime", cpuTime_.elapsedCpuTime());
    add("endDate", clock::date());
    add("endTime", clock::clockTime());
    if (!found("termination"))
    {
      add("termination", terminationType);
    }
    rm(runningJobPath_);
    write(OFstream(finishedJobPath_)());
  }
  constructed = false;
}
void mousse::JobInfo::end()
{
  end("normal");
}
void mousse::JobInfo::exit()
{
  end("exit");
}
void mousse::JobInfo::abort()
{
  end("abort");
}
void mousse::JobInfo::signalEnd() const
{
  if (writeJobInfo && constructed && Pstream::master())
  {
    mv(runningJobPath_, finishedJobPath_);
  }
  constructed = false;
}
