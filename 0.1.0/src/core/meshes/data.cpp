// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data.hpp"
#include "time.hpp"
#include "solver_performance.hpp"
// Static Data Members
int mousse::data::debug(mousse::debug::debugSwitch("data", false));
// Constructors 
mousse::data::data(const objectRegistry& obr)
:
  IOdictionary
  (
    IOobject
    (
      "data",
      obr.time().system(),
      obr,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    )
  ),
  prevTimeIndex_(0)
{
  set("solverPerformance", dictionary());
}
// Member Functions 
const mousse::dictionary& mousse::data::solverPerformanceDict() const
{
  return subDict("solverPerformance");
}
void mousse::data::setSolverPerformance
(
  const word& name,
  const solverPerformance& sp
) const
{
  dictionary& dict = const_cast<dictionary&>(solverPerformanceDict());
  List<solverPerformance> perfs;
  if (prevTimeIndex_ != this->time().timeIndex())
  {
    // reset solver performance between iterations
    prevTimeIndex_ = this->time().timeIndex();
    dict.clear();
  }
  else
  {
    dict.readIfPresent(name, perfs);
  }
  // append to list
  perfs.setSize(perfs.size()+1, sp);
  dict.set(name, perfs);
}
void mousse::data::setSolverPerformance
(
  const solverPerformance& sp
) const
{
  setSolverPerformance(sp.fieldName(), sp);
}
