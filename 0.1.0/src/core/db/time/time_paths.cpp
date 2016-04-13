// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_paths.hpp"
#include "iostreams.hpp"


// Constructors 
mousse::TimePaths::TimePaths
(
  const fileName& rootPath,
  const fileName& caseName,
  const word& systemName,
  const word& constantName
)
:
  processorCase_{false},
  rootPath_{rootPath},
  case_{caseName},
  system_{systemName},
  constant_{constantName}
{
  // Find out from case name whether a processor directory
  std::string::size_type pos = caseName.find("processor");
  if (pos != string::npos) {
    processorCase_ = true;
    if (pos == 0) {
      globalCaseName_ = ".";
    }
    else
    {
      globalCaseName_ = caseName(pos-1);
    }
  } else {
    globalCaseName_ = caseName;
  }
}


mousse::TimePaths::TimePaths
(
  const bool processorCase,
  const fileName& rootPath,
  const fileName& globalCaseName,
  const fileName& caseName,
  const word& systemName,
  const word& constantName
)
:
  processorCase_{processorCase},
  rootPath_{rootPath},
  globalCaseName_{globalCaseName},
  case_{caseName},
  system_{systemName},
  constant_{constantName}
{
  if (!processorCase) {
    // For convenience: find out from case name whether it is a
    // processor directory and set processorCase flag so file searching
    // goes up one level.
    std::string::size_type pos = caseName.find("processor");
    if (pos != string::npos) {
      processorCase_ = true;
      if (pos == 0) {
        globalCaseName_ = ".";
      } else {
        globalCaseName_ = caseName(pos-1);
      }
    }
  }
}


// Member Functions 
mousse::fileName mousse::TimePaths::caseSystem() const
{
  if (processorCase_) {
    return ".."/system();
  } else {
    return system();
  }
}


mousse::fileName mousse::TimePaths::caseConstant() const
{
  if (processorCase_) {
    return ".."/constant();
  } else {
    return constant();
  }
}
