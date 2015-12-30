// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_reader.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "empty_poly_patch.hpp"
#include "cell_modeller.hpp"
#include "demand_driven_data.hpp"
// Static Data Members
const mousse::cellModel* mousse::meshReader::unknownModel = mousse::cellModeller::
lookup
(
  "unknown"
);
const mousse::cellModel* mousse::meshReader::tetModel = mousse::cellModeller::
lookup
(
  "tet"
);
const mousse::cellModel* mousse::meshReader::pyrModel = mousse::cellModeller::
lookup
(
  "pyr"
);
const mousse::cellModel* mousse::meshReader::prismModel = mousse::cellModeller::
lookup
(
  "prism"
);
const mousse::cellModel* mousse::meshReader::hexModel = mousse::cellModeller::
lookup
(
  "hex"
);
// Private Member Functions 
void mousse::meshReader::addCellZones(polyMesh& mesh) const
{
  cellTable_.addCellZones(mesh, cellTableId_);
  warnDuplicates("cellZones", mesh.cellZones().names());
}
void mousse::meshReader::addFaceZones(polyMesh& mesh) const
{
  label nZone = monitoringSets_.size();
  mesh.faceZones().setSize(nZone);
  if (!nZone)
  {
    return;
  }
  nZone = 0;
  for
  (
    HashTable<List<label>, word, string::hash>::const_iterator
    iter = monitoringSets_.begin();
    iter != monitoringSets_.end();
    ++iter
  )
  {
    Info<< "faceZone " << nZone
      << " (size: " << iter().size() << ") name: "
      << iter.key() << endl;
    mesh.faceZones().set
    (
      nZone,
      new faceZone
      (
        iter.key(),
        iter(),
        List<bool>(iter().size(), false),
        nZone,
        mesh.faceZones()
      )
    );
    nZone++;
  }
  mesh.faceZones().writeOpt() = IOobject::AUTO_WRITE;
  warnDuplicates("faceZones", mesh.faceZones().names());
}
mousse::autoPtr<mousse::polyMesh> mousse::meshReader::mesh
(
  const objectRegistry& registry
)
{
  readGeometry();
  Info<< "Creating a polyMesh" << endl;
  createPolyCells();
  Info<< "Number of internal faces: " << nInternalFaces_ << endl;
  createPolyBoundary();
  clearExtraStorage();
  autoPtr<polyMesh> mesh
  (
    new polyMesh
    (
      IOobject
      (
        polyMesh::defaultRegion,
        registry.time().constant(),
        registry
      ),
      xferMove(points_),
      xferMove(meshFaces_),
      xferMove(cellPolys_)
    )
  );
  // adding patches also checks the mesh
  mesh().addPatches(polyBoundaryPatches(mesh));
  warnDuplicates("boundaries", mesh().boundaryMesh().names());
  addCellZones(mesh());
  addFaceZones(mesh());
  return mesh;
}
void mousse::meshReader::writeMesh
(
  const polyMesh& mesh,
  IOstream::streamFormat fmt
) const
{
  mesh.removeFiles();
  Info<< "Writing polyMesh" << endl;
  mesh.writeObject
  (
    fmt,
    IOstream::currentVersion,
    IOstream::UNCOMPRESSED
  );
  writeAux(mesh);
}
void mousse::meshReader::clearExtraStorage()
{
  cellFaces_.clear();
  baffleFaces_.clear();
  boundaryIds_.clear();
  baffleIds_.clear();
  deleteDemandDrivenData(pointCellsPtr_);
}
// Constructors 
mousse::meshReader::meshReader
(
  const fileName& fileOrPrefix,
  const scalar scaleFactor
)
  :
  pointCellsPtr_(NULL),
  nInternalFaces_(0),
  patchStarts_(0),
  patchSizes_(0),
  interfaces_(0),
  baffleIds_(0),
  meshFaces_(0),
  cellPolys_(0),
  geometryFile_(fileOrPrefix),
  scaleFactor_(scaleFactor),
  points_(0),
  origCellId_(0),
  boundaryIds_(0),
  patchTypes_(0),
  patchNames_(0),
  patchPhysicalTypes_(0),
  cellFaces_(0),
  baffleFaces_(0),
  cellTableId_(0),
  cellTable_()
{}
// Destructor 
mousse::meshReader::~meshReader()
{
  deleteDemandDrivenData(pointCellsPtr_);
}
