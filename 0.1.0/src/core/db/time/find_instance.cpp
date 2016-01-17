// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time.hpp"
#include "ioobject.hpp"
mousse::word mousse::Time::findInstance
(
  const fileName& dir,
  const word& name,
  const IOobject::readOption rOpt,
  const word& stopInstance
) const
{
  // Note: if name is empty, just check the directory itself
  const fileName tPath(path());
  const fileName dirPath(tPath/timeName()/dir);
  // check the current time directory
  if
  (
    name.empty()
   ? isDir(dirPath)
   :
    (
      isFile(dirPath/name)
    && IOobject(name, timeName(), dir, *this).headerOk()
    )
  )
  {
    if (debug)
    {
      Info<< "Time::findInstance"
        "(const fileName&, const word&"
        ", const IOobject::readOption, const word&)"
        << " : found \"" << name
        << "\" in " << timeName()/dir
        << endl;
    }
    return timeName();
  }
  // Search back through the time directories to find the time
  // closest to and lower than current time
  instantList ts = times();
  label instanceI;
  for (instanceI = ts.size()-1; instanceI >= 0; --instanceI)
  {
    if (ts[instanceI].value() <= timeOutputValue())
    {
      break;
    }
  }
  // continue searching from here
  for (; instanceI >= 0; --instanceI)
  {
    if
    (
      name.empty()
     ? isDir(tPath/ts[instanceI].name()/dir)
     :
      (
        isFile(tPath/ts[instanceI].name()/dir/name)
      && IOobject(name, ts[instanceI].name(), dir, *this).headerOk()
      )
    )
    {
      if (debug)
      {
        Info<< "Time::findInstance"
          "(const fileName&, const word&"
          ", const IOobject::readOption, const word&)"
          << " : found \"" << name
          << "\" in " << ts[instanceI].name()/dir
          << endl;
      }
      return ts[instanceI].name();
    }
    // Check if hit minimum instance
    if (ts[instanceI].name() == stopInstance)
    {
      if (debug)
      {
        Info<< "Time::findInstance"
          "(const fileName&, const word&"
          ", const IOobject::readOption, const word&)"
          << " : hit stopInstance " << stopInstance
          << endl;
      }
      if
      (
        rOpt == IOobject::MUST_READ
      || rOpt == IOobject::MUST_READ_IF_MODIFIED
      )
      {
        if (name.empty())
        {
          FATAL_ERROR_IN
          (
            "Time::findInstance"
            "(const fileName&, const word&"
            ", const IOobject::readOption, const word&)"
          )   << "Cannot find directory "
            << dir << " in times " << timeName()
            << " down to " << stopInstance
            << exit(FatalError);
        }
        else
        {
          FATAL_ERROR_IN
          (
            "Time::findInstance"
            "(const fileName&, const word&"
            ", const IOobject::readOption, const word&)"
          )   << "Cannot find file \"" << name << "\" in directory "
            << dir << " in times " << timeName()
            << " down to " << stopInstance
            << exit(FatalError);
        }
      }
      return ts[instanceI].name();
    }
  }
  // not in any of the time directories, try constant
  // Note. This needs to be a hard-coded constant, rather than the
  // constant function of the time, because the latter points to
  // the case constant directory in parallel cases
  if
  (
    name.empty()
   ? isDir(tPath/constant()/dir)
   :
    (
      isFile(tPath/constant()/dir/name)
    && IOobject(name, constant(), dir, *this).headerOk()
    )
  )
  {
    if (debug)
    {
      Info<< "Time::findInstance"
        "(const fileName&, const word&"
        ", const IOobject::readOption, const word&)"
        << " : found \"" << name
        << "\" in " << constant()/dir
        << endl;
    }
    return constant();
  }
  if (rOpt == IOobject::MUST_READ || rOpt == IOobject::MUST_READ_IF_MODIFIED)
  {
    FATAL_ERROR_IN
    (
      "Time::findInstance"
      "(const fileName&, const word&"
      ", const IOobject::readOption, const word&)"
    )   << "Cannot find file \"" << name << "\" in directory "
      << dir << " in times " << timeName()
      << " down to " << constant()
      << exit(FatalError);
  }
  return constant();
}
