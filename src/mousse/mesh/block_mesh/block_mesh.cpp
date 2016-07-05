// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "block_mesh.hpp"
#include "switch.hpp"


// Static Data Members
bool mousse::blockMesh::blockMesh::verboseOutput{false};


namespace mousse {

DEFINE_DEBUG_SWITCH(blockMesh, 0);

}


// Constructors 
mousse::blockMesh::blockMesh(const IOdictionary& dict, const word& regionName)
:
  blockPointField_{dict.lookup("vertices")},
  scaleFactor_{1.0},
  topologyPtr_{createTopology(dict, regionName)}
{
  Switch fastMerge{dict.lookupOrDefault<Switch>("fastMerge", false)};
  if (fastMerge) {
    calcMergeInfoFast();
  } else {
    calcMergeInfo();
  }
}


// Destructor 
mousse::blockMesh::~blockMesh()
{
  delete topologyPtr_;
}


// Member Functions 
void mousse::blockMesh::verbose(const bool on)
{
  verboseOutput = on;
}


const mousse::pointField& mousse::blockMesh::blockPointField() const
{
  return blockPointField_;
}


const mousse::polyMesh& mousse::blockMesh::topology() const
{
  if (!topologyPtr_) {
    FATAL_ERROR_IN("blockMesh::topology() const")
      << "topologyPtr_ not allocated"
      << exit(FatalError);
  }
  return *topologyPtr_;
}


mousse::PtrList<mousse::dictionary> mousse::blockMesh::patchDicts() const
{
  const polyPatchList& patchTopologies = topology().boundaryMesh();
  PtrList<dictionary> patchDicts{patchTopologies.size()};
  FOR_ALL(patchTopologies, patchI) {
    OStringStream os;
    patchTopologies[patchI].write(os);
    IStringStream is{os.str()};
    patchDicts.set(patchI, new dictionary{is});
  }
  return patchDicts;
}


mousse::scalar mousse::blockMesh::scaleFactor() const
{
  return scaleFactor_;
}


const mousse::pointField& mousse::blockMesh::points() const
{
  if (points_.empty()) {
    createPoints();
  }
  return points_;
}


const mousse::cellShapeList& mousse::blockMesh::cells() const
{
  if (cells_.empty()) {
    createCells();
  }
  return cells_;
}


const mousse::faceListList& mousse::blockMesh::patches() const
{
  if (patches_.empty()) {
    createPatches();
  }
  return patches_;
}


mousse::wordList mousse::blockMesh::patchNames() const
{
  return topology().boundaryMesh().names();
}


mousse::label mousse::blockMesh::numZonedBlocks() const
{
  label num = 0;
  FOR_ALL(*this, blockI) {
    if (operator[](blockI).zoneName().size()) {
      num++;
    }
  }
  return num;
}


void mousse::blockMesh::writeTopology(Ostream& os) const
{
  const pointField& pts = topology().points();
  FOR_ALL(pts, pI) {
    const point& pt = pts[pI];
    os << "v " << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
  }
  const edgeList& edges = topology().edges();
  FOR_ALL(edges, eI) {
    const edge& e = edges[eI];
    os << "l " << e.start() + 1 << ' ' << e.end() + 1 << endl;
  }
}

