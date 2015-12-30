// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tether_potential_list.hpp"
// Private Member Functions 
void mousse::tetherPotentialList::readTetherPotentialDict
(
  const List<word>& siteIdList,
  const dictionary& tetherPotentialDict,
  const List<word>& tetherSiteIdList
)
{
  Info<< nl << "Building tether potentials." << endl;
  idMap_ = List<label>(siteIdList.size(), -1);
  label tetherMapIndex = 0;
  forAll(tetherSiteIdList, t)
  {
    word tetherPotentialName = tetherSiteIdList[t];
    label tetherId = findIndex(siteIdList, tetherPotentialName);
    if (tetherId == -1)
    {
      FatalErrorIn("tetherPotentialList::readTetherPotentialDict")
        << nl
        << "No matching entry found in siteIdList for tether name "
        << tetherPotentialName
        << abort(FatalError);
    }
    else if (!tetherPotentialDict.found(tetherPotentialName))
    {
      FatalErrorIn("tetherPotentialList::readTetherPotentialDict")
        << nl << "tether potential specification subDict "
        << tetherPotentialName << " not found"
        << abort(FatalError);
    }
    else
    {
      this->set
      (
        tetherMapIndex,
        tetherPotential::New
        (
          tetherPotentialName,
          tetherPotentialDict.subDict(tetherPotentialName)
        )
      );
    }
    idMap_[tetherId] = tetherMapIndex;
    tetherMapIndex++;
  }
}
// Constructors 
mousse::tetherPotentialList::tetherPotentialList()
:
  PtrList<tetherPotential>(),
  idMap_()
{}
mousse::tetherPotentialList::tetherPotentialList
(
  const List<word>& siteIdList,
  const dictionary& tetherPotentialDict,
  const List<word>& tetherSiteIdList
)
:
  PtrList<tetherPotential>(),
  idMap_()
{
  buildPotentials(siteIdList, tetherPotentialDict, tetherSiteIdList);
}
// Destructor 
mousse::tetherPotentialList::~tetherPotentialList()
{}
// Member Functions 
void mousse::tetherPotentialList::buildPotentials
(
  const List<word>& siteIdList,
  const dictionary& tetherPotentialDict,
  const List<word>& tetherSiteIdList
)
{
  setSize(tetherSiteIdList.size());
  readTetherPotentialDict(siteIdList, tetherPotentialDict, tetherSiteIdList);
}
const mousse::tetherPotential& mousse::tetherPotentialList::tetherPotentialFunction
(
  const label a
) const
{
  return (*this)[tetherPotentialIndex(a)];
}
mousse::vector mousse::tetherPotentialList::force
(
  const label a,
  const vector rIT
) const
{
  return (*this)[tetherPotentialIndex(a)].force(rIT);
}
mousse::scalar mousse::tetherPotentialList::energy
(
  const label a,
  const vector rIT
) const
{
  return (*this)[tetherPotentialIndex(a)].energy(rIT);
}
