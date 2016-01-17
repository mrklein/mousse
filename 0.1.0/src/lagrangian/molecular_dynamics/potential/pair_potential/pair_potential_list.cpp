// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_potential_list.hpp"
#include "ofstream.hpp"
#include "time.hpp"
// Private Member Functions 
void mousse::pairPotentialList::readPairPotentialDict
(
  const List<word>& idList,
  const dictionary& pairPotentialDict,
  const polyMesh& mesh
)
{
  Info<< nl << "Building pair potentials." << endl;
  rCutMax_ = 0.0;
  for (label a = 0; a < nIds_; ++a)
  {
    word idA = idList[a];
    for (label b = a; b < nIds_; ++b)
    {
      word idB = idList[b];
      word pairPotentialName;
      if (a == b)
      {
        if (pairPotentialDict.found(idA + "-" + idB))
        {
          pairPotentialName = idA + "-" + idB;
        }
        else
        {
          FATAL_ERROR_IN("pairPotentialList::buildPotentials") << nl
            << "Pair pairPotential specification subDict "
            << idA << "-" << idB << " not found"
            << nl << abort(FatalError);
        }
      }
      else
      {
        if (pairPotentialDict.found(idA + "-" + idB))
        {
          pairPotentialName = idA + "-" + idB;
        }
        else if (pairPotentialDict.found(idB + "-" + idA))
        {
          pairPotentialName = idB + "-" + idA;
        }
        else
        {
          FATAL_ERROR_IN("pairPotentialList::buildPotentials") << nl
            << "Pair pairPotential specification subDict "
            << idA << "-" << idB << " or "
            << idB << "-" << idA << " not found"
            << nl << abort(FatalError);
        }
        if
        (
          pairPotentialDict.found(idA+"-"+idB)
        && pairPotentialDict.found(idB+"-"+idA)
        )
        {
          FATAL_ERROR_IN("pairPotentialList::buildPotentials") << nl
            << "Pair pairPotential specification subDict "
            << idA << "-" << idB << " and "
            << idB << "-" << idA << " found multiple definition"
            << nl << abort(FatalError);
        }
      }
      (*this).set
      (
        pairPotentialIndex(a, b),
        pairPotential::New
        (
          pairPotentialName,
          pairPotentialDict.subDict(pairPotentialName)
        )
      );
      if ((*this)[pairPotentialIndex(a, b)].rCut() > rCutMax_)
      {
        rCutMax_ = (*this)[pairPotentialIndex(a, b)].rCut();
      }
      if ((*this)[pairPotentialIndex(a, b)].writeTables())
      {
        OFstream ppTabFile(mesh.time().path()/pairPotentialName);
        if
        (
          !(*this)[pairPotentialIndex(a, b)].writeEnergyAndForceTables
          (
            ppTabFile
          )
        )
        {
          FATAL_ERROR_IN("pairPotentialList::readPairPotentialDict")
            << "Failed writing to "
            << ppTabFile.name() << nl
            << abort(FatalError);
        }
      }
    }
  }
  if (!pairPotentialDict.found("electrostatic"))
  {
    FATAL_ERROR_IN("pairPotentialList::buildPotentials") << nl
      << "Pair pairPotential specification subDict electrostatic"
      << nl << abort(FatalError);
  }
  electrostaticPotential_ = pairPotential::New
  (
    "electrostatic",
    pairPotentialDict.subDict("electrostatic")
  );
  if (electrostaticPotential_->rCut() > rCutMax_)
  {
    rCutMax_ = electrostaticPotential_->rCut();
  }
  if (electrostaticPotential_->writeTables())
  {
    OFstream ppTabFile(mesh.time().path()/"electrostatic");
    if (!electrostaticPotential_->writeEnergyAndForceTables(ppTabFile))
    {
      FATAL_ERROR_IN("pairPotentialList::readPairPotentialDict")
        << "Failed writing to "
        << ppTabFile.name() << nl
        << abort(FatalError);
    }
  }
  rCutMaxSqr_ = rCutMax_*rCutMax_;
}
// Constructors 
mousse::pairPotentialList::pairPotentialList()
:
  PtrList<pairPotential>()
{}
mousse::pairPotentialList::pairPotentialList
(
  const List<word>& idList,
  const dictionary& pairPotentialDict,
  const polyMesh& mesh
)
:
  PtrList<pairPotential>()
{
  buildPotentials(idList, pairPotentialDict, mesh);
}
// Destructor 
mousse::pairPotentialList::~pairPotentialList()
{}
// Member Functions 
void mousse::pairPotentialList::buildPotentials
(
  const List<word>& idList,
  const dictionary& pairPotentialDict,
  const polyMesh& mesh
)
{
  setSize(((idList.size()*(idList.size() + 1))/2));
  nIds_ = idList.size();
  readPairPotentialDict(idList, pairPotentialDict, mesh);
}
const mousse::pairPotential& mousse::pairPotentialList::pairPotentialFunction
(
  const label a,
  const label b
) const
{
  return (*this)[pairPotentialIndex(a, b)];
}
bool mousse::pairPotentialList::rCutMaxSqr(const scalar rIJMagSqr) const
{
  if (rIJMagSqr < rCutMaxSqr_)
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool mousse::pairPotentialList::rCutSqr
(
  const label a,
  const label b,
  const scalar rIJMagSqr
) const
{
  if (rIJMagSqr < rCutSqr(a, b))
  {
    return true;
  }
  else
  {
    return false;
  }
}
mousse::scalar mousse::pairPotentialList::rMin
(
  const label a,
  const label b
) const
{
  return (*this)[pairPotentialIndex(a, b)].rMin();
}
mousse::scalar mousse::pairPotentialList::dr
(
  const label a,
  const label b
) const
{
  return (*this)[pairPotentialIndex(a, b)].dr();
}
mousse::scalar mousse::pairPotentialList::rCutSqr
(
  const label a,
  const label b
) const
{
  return (*this)[pairPotentialIndex(a, b)].rCutSqr();
}
mousse::scalar mousse::pairPotentialList::rCut
(
  const label a,
  const label b
) const
{
  return (*this)[pairPotentialIndex(a, b)].rCut();
}
mousse::scalar mousse::pairPotentialList::force
(
  const label a,
  const label b,
  const scalar rIJMag
) const
{
  scalar f = (*this)[pairPotentialIndex(a, b)].force(rIJMag);
  return f;
}
mousse::scalar mousse::pairPotentialList::energy
(
  const label a,
  const label b,
  const scalar rIJMag
) const
{
  scalar e = (*this)[pairPotentialIndex(a, b)].energy(rIJMag);
  return e;
}
