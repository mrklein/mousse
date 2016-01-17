// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sub_cycle_time.hpp"
// Constructors 
mousse::subCycleTime::subCycleTime(Time& t, const label nSubCycles)
:
  time_(t),
  nSubCycles_(nSubCycles),
  subCycleIndex_(0)
{
  time_.subCycle(nSubCycles_);
}
// Destructor 
mousse::subCycleTime::~subCycleTime()
{
  endSubCycle();
}
// Member Functions 
bool mousse::subCycleTime::end() const
{
  return subCycleIndex_ > nSubCycles_;
}
void mousse::subCycleTime::endSubCycle()
{
  time_.endSubCycle();
}
// Member Operators 
mousse::subCycleTime& mousse::subCycleTime::operator++()
{
  time_++;
  subCycleIndex_++;
  return *this;
}
mousse::subCycleTime& mousse::subCycleTime::operator++(int)
{
  return operator++();
}
