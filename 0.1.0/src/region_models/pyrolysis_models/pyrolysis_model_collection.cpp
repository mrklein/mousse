// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pyrolysis_model_collection.hpp"
#include "vol_fields.hpp"
// Static Data Members
namespace mousse
{
  namespace regionModels
  {
    namespace pyrolysisModels
    {
      DEFINE_TYPE_NAME_AND_DEBUG(pyrolysisModelCollection, 0);
    }
  }
}
namespace mousse
{
namespace regionModels
{
namespace pyrolysisModels
{
// Constructors 
pyrolysisModelCollection::pyrolysisModelCollection(const fvMesh& mesh)
:
  PtrList<pyrolysisModel>()
{
  IOdictionary pyrolysisZonesDict
  (
    IOobject
    (
      "pyrolysisZones",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    )
  );
  const wordList regions(pyrolysisZonesDict.toc());
  setSize(regions.size());
  for (label i = 0; i < regions.size(); i++)
  {
    set
    (
      i,
      pyrolysisModel::New
      (
        mesh,
        pyrolysisZonesDict.subDict(regions[i]),
        regions[i]
      )
    );
  }
}
// Destructor 
pyrolysisModelCollection::~pyrolysisModelCollection()
{}
// Member Functions 
void pyrolysisModelCollection::preEvolveRegion()
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).preEvolveRegion();
  }
}
void pyrolysisModelCollection::evolveRegion()
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).evolveRegion();
  }
}
void pyrolysisModelCollection::evolve()
{
  FOR_ALL(*this, i)
  {
    pyrolysisModel& pyrolysis = this->operator[](i);
    if (pyrolysis.active())
    {
      if (pyrolysis.primaryMesh().changing())
      {
        FATAL_ERROR_IN("pyrolysisModelCollection::evolve()")
          << "Currently not possible to apply "
          << pyrolysis.modelName()
          << " model to moving mesh cases" << nl<< abort(FatalError);
      }
      // Pre-evolve
      pyrolysis.preEvolveRegion();
      // Increment the region equations up to the new time level
      pyrolysis.evolveRegion();
      // Provide some feedback
      if (pyrolysis.infoOutput())
      {
        Info<< incrIndent;
        pyrolysis.info();
        Info<< endl << decrIndent;
      }
    }
  }
}
void pyrolysisModelCollection::info()
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).info();
  }
}
scalar pyrolysisModelCollection::maxDiff() const
{
  scalar maxDiff = 0.0;
  FOR_ALL(*this, i)
  {
    if (maxDiff < this->operator[](i).maxDiff())
    {
      maxDiff = this->operator[](i).maxDiff();
    }
  }
  return maxDiff;
}
scalar pyrolysisModelCollection::solidRegionDiffNo() const
{
  scalar totalDiNum = GREAT;
  FOR_ALL(*this, i)
  {
    if
    (
      totalDiNum > this->operator[](i).solidRegionDiffNo()
    )
    {
      totalDiNum = this->operator[](i).solidRegionDiffNo();
    }
  }
  return totalDiNum;
}
}  // namespace pyrolysisModels
}  // namespace regionModels
}  // namespace mousse
