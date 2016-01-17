// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surfaces.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "iomanip.hpp"
#include "vol_point_interpolation.hpp"
#include "patch_tools.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(sampledSurfaces, 0);
}
bool mousse::sampledSurfaces::verbose_ = false;
mousse::scalar mousse::sampledSurfaces::mergeTol_ = 1e-10;
// Private Member Functions 
void mousse::sampledSurfaces::writeGeometry() const
{
  // Write to time directory under outputPath_
  // Skip surface without faces (eg, a failed cut-plane)
  const fileName outputDir = outputPath_/mesh_.time().timeName();
  FOR_ALL(*this, surfI)
  {
    const sampledSurface& s = operator[](surfI);
    if (Pstream::parRun())
    {
      if (Pstream::master() && mergeList_[surfI].faces.size())
      {
        formatter_->write
        (
          outputDir,
          s.name(),
          mergeList_[surfI].points,
          mergeList_[surfI].faces
        );
      }
    }
    else if (s.faces().size())
    {
      formatter_->write
      (
        outputDir,
        s.name(),
        s.points(),
        s.faces()
      );
    }
  }
}
// Constructors 
mousse::sampledSurfaces::sampledSurfaces
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  PtrList<sampledSurface>(),
  name_(name),
  mesh_(refCast<const fvMesh>(obr)),
  loadFromFiles_(loadFromFiles),
  outputPath_(fileName::null),
  fieldSelection_(),
  interpolationScheme_(word::null),
  mergeList_(),
  formatter_(NULL)
{
  if (Pstream::parRun())
  {
    outputPath_ = mesh_.time().path()/".."/"postProcessing"/name_;
  }
  else
  {
    outputPath_ = mesh_.time().path()/"postProcessing"/name_;
  }
  read(dict);
}
// Destructor 
mousse::sampledSurfaces::~sampledSurfaces()
{}
// Member Functions 
void mousse::sampledSurfaces::verbose(const bool verbosity)
{
  verbose_ = verbosity;
}
void mousse::sampledSurfaces::execute()
{
  // Do nothing - only valid on write
}
void mousse::sampledSurfaces::end()
{
  // Do nothing - only valid on write
}
void mousse::sampledSurfaces::timeSet()
{
  // Do nothing - only valid on write
}
void mousse::sampledSurfaces::write()
{
  if (size())
  {
    // Finalize surfaces, merge points etc.
    update();
    const label nFields = classifyFields();
    if (Pstream::master())
    {
      if (debug)
      {
        Pout<< "Creating directory "
          << outputPath_/mesh_.time().timeName() << nl << endl;
      }
      mkDir(outputPath_/mesh_.time().timeName());
    }
    // Write geometry first if required,
    // or when no fields would otherwise be written
    if (nFields == 0 || formatter_->separateGeometry())
    {
      writeGeometry();
    }
    const IOobjectList objects(mesh_, mesh_.time().timeName());
    sampleAndWrite<volScalarField>(objects);
    sampleAndWrite<volVectorField>(objects);
    sampleAndWrite<volSphericalTensorField>(objects);
    sampleAndWrite<volSymmTensorField>(objects);
    sampleAndWrite<volTensorField>(objects);
    sampleAndWrite<surfaceScalarField>(objects);
    sampleAndWrite<surfaceVectorField>(objects);
    sampleAndWrite<surfaceSphericalTensorField>(objects);
    sampleAndWrite<surfaceSymmTensorField>(objects);
    sampleAndWrite<surfaceTensorField>(objects);
  }
}
void mousse::sampledSurfaces::read(const dictionary& dict)
{
  bool surfacesFound = dict.found("surfaces");
  if (surfacesFound)
  {
    dict.lookup("fields") >> fieldSelection_;
    dict.lookup("interpolationScheme") >> interpolationScheme_;
    const word writeType(dict.lookup("surfaceFormat"));
    // Define the surface formatter
    // Optionally defined extra controls for the output formats
    formatter_ = surfaceWriter::New
    (
      writeType,
      dict.subOrEmptyDict("formatOptions").subOrEmptyDict(writeType)
    );
    PtrList<sampledSurface> newList
    (
      dict.lookup("surfaces"),
      sampledSurface::iNew(mesh_)
    );
    transfer(newList);
    if (Pstream::parRun())
    {
      mergeList_.setSize(size());
    }
    // Ensure all surfaces and merge information are expired
    expire();
    if (this->size())
    {
      Info<< "Reading surface description:" << nl;
      FOR_ALL(*this, surfI)
      {
        Info<< "    " << operator[](surfI).name() << nl;
      }
      Info<< endl;
    }
  }
  if (Pstream::master() && debug)
  {
    Pout<< "sample fields:" << fieldSelection_ << nl
      << "sample surfaces:" << nl << "(" << nl;
    FOR_ALL(*this, surfI)
    {
      Pout<< "  " << operator[](surfI) << endl;
    }
    Pout<< ")" << endl;
  }
}
void mousse::sampledSurfaces::updateMesh(const mapPolyMesh&)
{
  expire();
  // pointMesh and interpolation will have been reset in mesh.update
}
void mousse::sampledSurfaces::movePoints(const polyMesh&)
{
  expire();
}
void mousse::sampledSurfaces::readUpdate(const polyMesh::readUpdateState state)
{
  if (state != polyMesh::UNCHANGED)
  {
    expire();
  }
}
bool mousse::sampledSurfaces::needsUpdate() const
{
  FOR_ALL(*this, surfI)
  {
    if (operator[](surfI).needsUpdate())
    {
      return true;
    }
  }
  return false;
}
bool mousse::sampledSurfaces::expire()
{
  bool justExpired = false;
  FOR_ALL(*this, surfI)
  {
    if (operator[](surfI).expire())
    {
      justExpired = true;
    }
    // Clear merge information
    if (Pstream::parRun())
    {
      mergeList_[surfI].clear();
    }
  }
  // true if any surfaces just expired
  return justExpired;
}
bool mousse::sampledSurfaces::update()
{
  bool updated = false;
  if (!needsUpdate())
  {
    return updated;
  }
  // Serial: quick and easy, no merging required
  if (!Pstream::parRun())
  {
    FOR_ALL(*this, surfI)
    {
      if (operator[](surfI).update())
      {
        updated = true;
      }
    }
    return updated;
  }
  // Dimension as fraction of mesh bounding box
  scalar mergeDim = mergeTol_ * mesh_.bounds().mag();
  if (Pstream::master() && debug)
  {
    Pout<< nl << "Merging all points within "
      << mergeDim << " metre" << endl;
  }
  FOR_ALL(*this, surfI)
  {
    sampledSurface& s = operator[](surfI);
    if (s.update())
    {
      updated = true;
    }
    else
    {
      continue;
    }
    PatchTools::gatherAndMerge
    (
      mergeDim,
      primitivePatch
      (
        SubList<face>(s.faces(), s.faces().size()),
        s.points()
      ),
      mergeList_[surfI].points,
      mergeList_[surfI].faces,
      mergeList_[surfI].pointsMap
    );
  }
  return updated;
}
