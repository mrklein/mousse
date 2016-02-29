#ifndef CORE_DB_TIME_SUB_CYCLE_TIME_HPP_
#define CORE_DB_TIME_SUB_CYCLE_TIME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::subCycleTime
// Description
//   A class for managing sub-cycling times
// SourceFiles
//   sub_cycle_time.cpp
#include "time.hpp"
namespace mousse
{
class subCycleTime
{
  // Private data
    Time& time_;
    label nSubCycles_;
    label subCycleIndex_;
public:
  // Constructors
    //- Construct from original time and number of sub-cycles
    subCycleTime(Time&, const label nSubCycles);
  //- Destructor
  ~subCycleTime();
  // Member functions
    //- Return true if the number of sub-cycles has been reached
    bool end() const;
    //- End the sub-cycling and reset the time-state
    void endSubCycle();
    //- Return the total number of sub-cycles
    label nSubCycles() const
    {
      return nSubCycles_;
    }
    //- Return the current sub-cycle index
    label index() const
    {
      return subCycleIndex_;
    }
  // Member operators
    //- Prefix increment
    subCycleTime& operator++();
    //- Postfix increment
    subCycleTime& operator++(int);
};
}  // namespace mousse
#endif
