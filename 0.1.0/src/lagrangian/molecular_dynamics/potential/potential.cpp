// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "potential.hpp"
// Private Member Functions 
void mousse::potential::setSiteIdList(const dictionary& moleculePropertiesDict)
{
  DynamicList<word> siteIdList;
  DynamicList<word> pairPotentialSiteIdList;
  forAll(idList_, i)
  {
    const word& id(idList_[i]);
    if (!moleculePropertiesDict.found(id))
    {
      FatalErrorIn("potential::setSiteIdList(const dictionary&)")
        << id << " molecule subDict not found"
        << nl << abort(FatalError);
    }
    const dictionary& molDict(moleculePropertiesDict.subDict(id));
    List<word> siteIdNames = molDict.lookup("siteIds");
    forAll(siteIdNames, sI)
    {
      const word& siteId = siteIdNames[sI];
      if (findIndex(siteIdList, siteId) == -1)
      {
        siteIdList.append(siteId);
      }
    }
    List<word> pairPotSiteIds = molDict.lookup("pairPotentialSiteIds");
    forAll(pairPotSiteIds, sI)
    {
      const word& siteId = pairPotSiteIds[sI];
      if (findIndex(siteIdNames, siteId) == -1)
      {
        FatalErrorIn("potential::setSiteIdList(const dictionary&)")
          << siteId << " in pairPotentialSiteIds is not in siteIds: "
          << siteIdNames << nl << abort(FatalError);
      }
      if (findIndex(pairPotentialSiteIdList, siteId) == -1)
      {
        pairPotentialSiteIdList.append(siteId);
      }
    }
  }
  nPairPotIds_ = pairPotentialSiteIdList.size();
  forAll(siteIdList, aSIN)
  {
    const word& siteId = siteIdList[aSIN];
    if (findIndex(pairPotentialSiteIdList, siteId) == -1)
    {
      pairPotentialSiteIdList.append(siteId);
    }
  }
  siteIdList_.transfer(pairPotentialSiteIdList.shrink());
}
void mousse::potential::potential::readPotentialDict()
{
  Info<< nl <<  "Reading potential dictionary:" << endl;
  IOdictionary idListDict
  (
    IOobject
    (
      "idList",
      mesh_.time().constant(),
      mesh_,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  );
  idList_ = List<word>(idListDict.lookup("idList"));
  setSiteIdList
  (
    IOdictionary
    (
      IOobject
      (
        "moleculeProperties",
        mesh_.time().constant(),
        mesh_,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    )
  );
  List<word> pairPotentialSiteIdList
  (
    SubList<word>(siteIdList_, nPairPotIds_)
  );
  Info<< nl << "Unique site ids found: " << siteIdList_
    << nl << "Site Ids requiring a pair potential: "
    << pairPotentialSiteIdList
    << endl;
  List<word> tetherSiteIdList(0);
  if (idListDict.found("tetherSiteIdList"))
  {
    tetherSiteIdList = List<word>(idListDict.lookup("tetherSiteIdList"));
  }
  IOdictionary potentialDict
  (
    IOobject
    (
      "potentialDict",
      mesh_.time().system(),
      mesh_,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  );
  potentialEnergyLimit_ = readScalar
  (
    potentialDict.lookup("potentialEnergyLimit")
  );
  if (potentialDict.found("removalOrder"))
  {
    List<word> remOrd = potentialDict.lookup("removalOrder");
    removalOrder_.setSize(remOrd.size());
    forAll(removalOrder_, rO)
    {
      removalOrder_[rO] = findIndex(idList_, remOrd[rO]);
      if (removalOrder_[rO] == -1)
      {
        FatalErrorIn("potential::readPotentialDict()")
          << "removalOrder entry: " << remOrd[rO]
          << " not found in idList."
          << nl << abort(FatalError);
      }
    }
  }
  // *************************************************************************
  // Pair potentials
  if (!potentialDict.found("pair"))
  {
    FatalErrorIn("potential::readPotentialDict()")
      << "pair potential specification subDict not found"
      << abort(FatalError);
  }
  const dictionary& pairDict = potentialDict.subDict("pair");
  pairPotentials_.buildPotentials
  (
    pairPotentialSiteIdList,
    pairDict,
    mesh_
  );
  // *************************************************************************
  // Tether potentials
  if (tetherSiteIdList.size())
  {
    if (!potentialDict.found("tether"))
    {
      FatalErrorIn("potential::readPotentialDict()")
        << "tether potential specification subDict not found"
        << abort(FatalError);
    }
    const dictionary& tetherDict = potentialDict.subDict("tether");
    tetherPotentials_.buildPotentials
    (
      siteIdList_,
      tetherDict,
      tetherSiteIdList
    );
  }
  // *************************************************************************
  // External Forces
  gravity_ = vector::zero;
  if (potentialDict.found("external"))
  {
    Info<< nl << "Reading external forces:" << endl;
    const dictionary& externalDict = potentialDict.subDict("external");
    // gravity
    externalDict.readIfPresent("gravity", gravity_);
  }
  Info<< nl << tab << "gravity = " << gravity_ << endl;
}
void mousse::potential::potential::readMdInitialiseDict
(
  const IOdictionary& mdInitialiseDict,
  IOdictionary& idListDict
)
{
  IOdictionary moleculePropertiesDict
  (
    IOobject
    (
      "moleculeProperties",
      mesh_.time().constant(),
      mesh_,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    )
  );
  DynamicList<word> idList;
  DynamicList<word> tetherSiteIdList;
  forAll(mdInitialiseDict.toc(), zone)
  {
    const dictionary& zoneDict = mdInitialiseDict.subDict
    (
      mdInitialiseDict.toc()[zone]
    );
    List<word> latticeIds
    (
      zoneDict.lookup("latticeIds")
    );
    forAll(latticeIds, i)
    {
      const word& id = latticeIds[i];
      if (!moleculePropertiesDict.found(id))
      {
        FatalErrorIn
        (
          "potential::readMdInitialiseDict"
          "("
            "const IOdictionary&, "
            "IOdictionary&"
          ")"
        )   << "Molecule type " << id
          << " not found in moleculeProperties dictionary." << nl
          << abort(FatalError);
      }
      if (findIndex(idList,id) == -1)
      {
        idList.append(id);
      }
    }
    List<word> tetherSiteIds
    (
      zoneDict.lookup("tetherSiteIds")
    );
    forAll(tetherSiteIds, t)
    {
      const word& tetherSiteId = tetherSiteIds[t];
      bool idFound = false;
      forAll(latticeIds, i)
      {
        if (idFound)
        {
          break;
        }
        const word& id = latticeIds[i];
        List<word> siteIds
        (
          moleculePropertiesDict.subDict(id).lookup("siteIds")
        );
        if (findIndex(siteIds, tetherSiteId) != -1)
        {
          idFound = true;
        }
      }
      if (idFound)
      {
        tetherSiteIdList.append(tetherSiteId);
      }
      else
      {
        FatalErrorIn
        (
          "potential::readMdInitialiseDict"
          "("
            "const IOdictionary&, "
            "IOdictionary&"
          ")"
        )   << "Tether id  " << tetherSiteId
          << " not found as a site of any molecule in zone." << nl
          << abort(FatalError);
      }
    }
  }
  idList_.transfer(idList);
  tetherSiteIdList.shrink();
  idListDict.add("idList", idList_);
  idListDict.add("tetherSiteIdList", tetherSiteIdList);
  setSiteIdList(moleculePropertiesDict);
}
// Constructors 
mousse::potential::potential(const polyMesh& mesh)
:
  mesh_(mesh)
{
  readPotentialDict();
}
mousse::potential::potential
(
  const polyMesh& mesh,
  const IOdictionary& mdInitialiseDict,
  IOdictionary& idListDict
)
:
  mesh_(mesh)
{
  readMdInitialiseDict(mdInitialiseDict, idListDict);
}
// Destructor 
mousse::potential::~potential()
{}
