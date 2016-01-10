// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "list_list_ops.hpp"
#include "sortable_list.hpp"
#include "vol_point_interpolation.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sampledSets, 0);
bool sampledSets::verbose_ = false;
}
// Private Member Functions 
void mousse::sampledSets::combineSampledSets
(
  PtrList<coordSet>& masterSampledSets,
  labelListList& indexSets
)
{
  // Combine sampleSets from processors. Sort by curveDist. Return
  // ordering in indexSets.
  // Note: only master results are valid
  masterSampledSets_.clear();
  masterSampledSets_.setSize(size());
  indexSets_.setSize(size());
  const PtrList<sampledSet>& sampledSets = *this;
  FOR_ALL(sampledSets, setI)
  {
    const sampledSet& samplePts = sampledSets[setI];
    // Collect data from all processors
    List<List<point> > gatheredPts(Pstream::nProcs());
    gatheredPts[Pstream::myProcNo()] = samplePts;
    Pstream::gatherList(gatheredPts);
    List<labelList> gatheredSegments(Pstream::nProcs());
    gatheredSegments[Pstream::myProcNo()] = samplePts.segments();
    Pstream::gatherList(gatheredSegments);
    List<scalarList> gatheredDist(Pstream::nProcs());
    gatheredDist[Pstream::myProcNo()] = samplePts.curveDist();
    Pstream::gatherList(gatheredDist);
    // Combine processor lists into one big list.
    List<point> allPts
    (
      ListListOps::combine<List<point> >
      (
        gatheredPts, accessOp<List<point> >()
      )
    );
    labelList allSegments
    (
      ListListOps::combine<labelList>
      (
        gatheredSegments, accessOp<labelList>()
      )
    );
    scalarList allCurveDist
    (
      ListListOps::combine<scalarList>
      (
        gatheredDist, accessOp<scalarList>()
      )
    );
    if (Pstream::master() && allCurveDist.size() == 0)
    {
      WARNING_IN("sampledSets::combineSampledSets(..)")
        << "Sample set " << samplePts.name()
        << " has zero points." << endl;
    }
    // Sort curveDist and use to fill masterSamplePts
    SortableList<scalar> sortedDist(allCurveDist);
    indexSets[setI] = sortedDist.indices();
    masterSampledSets.set
    (
      setI,
      new coordSet
      (
        samplePts.name(),
        samplePts.axis(),
        List<point>(UIndirectList<point>(allPts, indexSets[setI])),
        allCurveDist
      )
    );
  }
}
// Constructors 
mousse::sampledSets::sampledSets
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  PtrList<sampledSet>(),
  name_(name),
  mesh_(refCast<const fvMesh>(obr)),
  loadFromFiles_(loadFromFiles),
  outputPath_(fileName::null),
  searchEngine_(mesh_),
  interpolationScheme_(word::null),
  writeFormat_(word::null)
{
  if (Pstream::parRun())
  {
    outputPath_ = mesh_.time().path()/".."/"postProcessing"/name_;
  }
  else
  {
    outputPath_ = mesh_.time().path()/"postProcessing"/name_;
  }
  if (mesh_.name() != fvMesh::defaultRegion)
  {
    outputPath_ = outputPath_/mesh_.name();
  }
  read(dict);
}
// Destructor 
mousse::sampledSets::~sampledSets()
{}
// Member Functions 
void mousse::sampledSets::verbose(const bool verbosity)
{
  verbose_ = verbosity;
}
void mousse::sampledSets::execute()
{
  // Do nothing - only valid on write
}
void mousse::sampledSets::end()
{
  // Do nothing - only valid on write
}
void mousse::sampledSets::timeSet()
{
  // Do nothing - only valid on write
}
void mousse::sampledSets::write()
{
  if (size())
  {
    const label nFields = classifyFields();
    if (Pstream::master())
    {
      if (debug)
      {
        Pout<< "timeName = " << mesh_.time().timeName() << nl
          << "scalarFields    " << scalarFields_ << nl
          << "vectorFields    " << vectorFields_ << nl
          << "sphTensorFields " << sphericalTensorFields_ << nl
          << "symTensorFields " << symmTensorFields_ <<nl
          << "tensorFields    " << tensorFields_ <<nl;
      }
      if (nFields)
      {
        if (debug)
        {
          Pout<< "Creating directory "
            << outputPath_/mesh_.time().timeName()
              << nl << endl;
        }
        mkDir(outputPath_/mesh_.time().timeName());
      }
    }
    if (nFields)
    {
      sampleAndWrite(scalarFields_);
      sampleAndWrite(vectorFields_);
      sampleAndWrite(sphericalTensorFields_);
      sampleAndWrite(symmTensorFields_);
      sampleAndWrite(tensorFields_);
    }
  }
}
void mousse::sampledSets::read(const dictionary& dict)
{
  dict_ = dict;
  bool setsFound = dict_.found("sets");
  if (setsFound)
  {
    dict_.lookup("fields") >> fieldSelection_;
    clearFieldGroups();
    dict.lookup("interpolationScheme") >> interpolationScheme_;
    dict.lookup("setFormat") >> writeFormat_;
    PtrList<sampledSet> newList
    (
      dict_.lookup("sets"),
      sampledSet::iNew(mesh_, searchEngine_)
    );
    transfer(newList);
    combineSampledSets(masterSampledSets_, indexSets_);
    if (this->size())
    {
      Info<< "Reading set description:" << nl;
      FOR_ALL(*this, setI)
      {
        Info<< "    " << operator[](setI).name() << nl;
      }
      Info<< endl;
    }
  }
  if (Pstream::master() && debug)
  {
    Pout<< "sample fields:" << fieldSelection_ << nl
      << "sample sets:" << nl << "(" << nl;
    FOR_ALL(*this, setI)
    {
      Pout<< "  " << operator[](setI) << endl;
    }
    Pout<< ")" << endl;
  }
}
void mousse::sampledSets::correct()
{
  bool setsFound = dict_.found("sets");
  if (setsFound)
  {
    searchEngine_.correct();
    PtrList<sampledSet> newList
    (
      dict_.lookup("sets"),
      sampledSet::iNew(mesh_, searchEngine_)
    );
    transfer(newList);
    combineSampledSets(masterSampledSets_, indexSets_);
  }
}
void mousse::sampledSets::updateMesh(const mapPolyMesh&)
{
  correct();
}
void mousse::sampledSets::movePoints(const polyMesh&)
{
  correct();
}
void mousse::sampledSets::readUpdate(const polyMesh::readUpdateState state)
{
  if (state != polyMesh::UNCHANGED)
  {
    correct();
  }
}
