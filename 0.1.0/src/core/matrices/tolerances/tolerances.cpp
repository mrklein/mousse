// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tolerances.hpp"
// Constructors 
mousse::tolerances::tolerances(const Time& t, const fileName& dictName)
:
  IOdictionary
  (
    IOobject
    (
      dictName,
      t.system(),
      t,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  ),
  relaxationFactors_(ITstream("relaxationFactors", tokenList())()),
  solverTolerances_(ITstream("solverTolerances", tokenList())()),
  solverRelativeTolerances_
  (
    ITstream("solverRelativeTolerances", tokenList())()
  )
{
  read();
}
// Member Functions 
bool mousse::tolerances::read()
{
  if (regIOobject::read())
  {
    const word toleranceSetName(lookup("toleranceSet"));
    const dictionary& toleranceSet(subDict(toleranceSetName));
    if (toleranceSet.found("relaxationFactors"))
    {
      relaxationFactors_ = toleranceSet.subDict("relaxationFactors");
    }
    if (toleranceSet.found("solverTolerances"))
    {
      solverTolerances_ = toleranceSet.subDict("solverTolerances");
    }
    if (toleranceSet.found("solverRelativeTolerances"))
    {
      solverRelativeTolerances_ =
        toleranceSet.subDict("solverRelativeTolerances");
    }
    return true;
  }
  else
  {
    return false;
  }
}
bool mousse::tolerances::relax(const word& name) const
{
  return relaxationFactors_.found(name);
}
mousse::scalar mousse::tolerances::relaxationFactor(const word& name) const
{
  return readScalar(relaxationFactors_.lookup(name));
}
mousse::scalar mousse::tolerances::solverTolerance(const word& name) const
{
  return readScalar(solverTolerances_.lookup(name));
}
bool mousse::tolerances::solverRelativeTolerances() const
{
  return solverRelativeTolerances_.size();
}
mousse::scalar mousse::tolerances::solverRelativeTolerance(const word& name) const
{
  return readScalar(solverRelativeTolerances_.lookup(name));
}
