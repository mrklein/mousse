// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_zone.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "face_zone_mesh.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "demand_driven_data.hpp"
#include "map_poly_mesh.hpp"
#include "sync_tools.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(faceZone, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(faceZone, dictionary);
ADD_TO_RUN_TIME_SELECTION_TABLE(faceZone, faceZone, dictionary);

}

const char* const mousse::faceZone::labelsName = "faceLabels";


// Private Member Functions
void mousse::faceZone::calcFaceZonePatch() const
{
  if (debug) {
    Info << "void faceZone::calcFaceZonePatch() const : "
      << "Calculating primitive patch"
      << endl;
  }
  if (patchPtr_) {
    FATAL_ERROR_IN
    (
      "void faceZone::calcFaceZonePatch() const"
    )
    << "primitive face zone patch already calculated"
    << abort(FatalError);
  }
  patchPtr_ =
    new primitiveFacePatch
    {
      faceList(size()),
      zoneMesh().mesh().points()
    };
  primitiveFacePatch& patch = *patchPtr_;
  const faceList& f = zoneMesh().mesh().faces();
  const labelList& addr = *this;
  const boolList& flip = flipMap();
  FOR_ALL(addr, faceI) {
    if (flip[faceI]) {
      patch[faceI] = f[addr[faceI]].reverseFace();
    } else {
      patch[faceI] = f[addr[faceI]];
    }
  }
  if (debug) {
    Info << "void faceZone::calcFaceZonePatch() const : "
      << "Finished calculating primitive patch"
      << endl;
  }
}


void mousse::faceZone::calcCellLayers() const
{
  if (debug) {
    Info << "void mousse::faceZone::calcCellLayers() const : "
      << "calculating master cells"
      << endl;
  }
  // It is an error to attempt to recalculate edgeCells
  // if the pointer is already set
  if (masterCellsPtr_ || slaveCellsPtr_) {
    FATAL_ERROR_IN("void faceZone::calcCellLayers() const")
      << "cell layers already calculated"
      << abort(FatalError);
  } else {
    // Go through all the faces in the master zone.  Choose the
    // master or slave cell based on the face flip
    const labelList& own = zoneMesh().mesh().faceOwner();
    const labelList& nei = zoneMesh().mesh().faceNeighbour();
    const labelList& mf = *this;
    const boolList& faceFlip = flipMap();
    masterCellsPtr_ = new labelList{mf.size()};
    labelList& mc = *masterCellsPtr_;
    slaveCellsPtr_ = new labelList{mf.size()};
    labelList& sc = *slaveCellsPtr_;
    FOR_ALL(mf, faceI) {
      label ownCellI = own[mf[faceI]];
      label neiCellI =
      (
        zoneMesh().mesh().isInternalFace(mf[faceI])
        ? nei[mf[faceI]]
        : -1
      );
      if (!faceFlip[faceI]) {
        // Face is oriented correctly, no flip needed
        mc[faceI] = neiCellI;
        sc[faceI] = ownCellI;
      } else {
        mc[faceI] = ownCellI;
        sc[faceI] = neiCellI;
      }
    }
  }
}


void mousse::faceZone::checkAddressing() const
{
  if (size() != flipMap_.size()) {
    FATAL_ERROR_IN("void mousse::faceZone::checkAddressing() const")
      << "Different sizes of the addressing and flipMap arrays.  "
      << "Size of addressing: " << size()
      << " size of flip map: " << flipMap_.size()
      << abort(FatalError);
  }
  const labelList& mf = *this;
  // Note: nFaces, nCells might not be set yet on mesh so use owner size
  const label nFaces = zoneMesh().mesh().faceOwner().size();
  bool hasWarned = false;
  FOR_ALL(mf, i) {
    if (!hasWarned && (mf[i] < 0 || mf[i] >= nFaces)) {
      WARNING_IN("void mousse::faceZone::checkAddressing() const")
        << "Illegal face index " << mf[i] << " outside range 0.."
        << nFaces-1 << endl;
      hasWarned = true;
    }
  }
}


// Constructors
// Construct from components
mousse::faceZone::faceZone
(
  const word& name,
  const labelUList& addr,
  const boolList& fm,
  const label index,
  const faceZoneMesh& zm
)
:
  zone{name, addr, index},
  flipMap_{fm},
  zoneMesh_{zm},
  patchPtr_{NULL},
  masterCellsPtr_{NULL},
  slaveCellsPtr_{NULL},
  mePtr_{NULL}
{
  checkAddressing();
}


mousse::faceZone::faceZone
(
  const word& name,
  const Xfer<labelList>& addr,
  const Xfer<boolList>& fm,
  const label index,
  const faceZoneMesh& zm
)
:
  zone{name, addr, index},
  flipMap_{fm},
  zoneMesh_{zm},
  patchPtr_{NULL},
  masterCellsPtr_{NULL},
  slaveCellsPtr_{NULL},
  mePtr_{NULL}
{
  checkAddressing();
}


mousse::faceZone::faceZone
(
  const word& name,
  const dictionary& dict,
  const label index,
  const faceZoneMesh& zm
)
:
  zone{name, dict, this->labelsName, index},
  flipMap_{dict.lookup("flipMap")},
  zoneMesh_{zm},
  patchPtr_{NULL},
  masterCellsPtr_{NULL},
  slaveCellsPtr_{NULL},
  mePtr_{NULL}
{
  checkAddressing();
}


mousse::faceZone::faceZone
(
  const faceZone& fz,
  const labelUList& addr,
  const boolList& fm,
  const label index,
  const faceZoneMesh& zm
)
:
  zone{fz, addr, index},
  flipMap_{fm},
  zoneMesh_{zm},
  patchPtr_{NULL},
  masterCellsPtr_{NULL},
  slaveCellsPtr_{NULL},
  mePtr_{NULL}
{
  checkAddressing();
}


mousse::faceZone::faceZone
(
  const faceZone& fz,
  const Xfer<labelList>& addr,
  const Xfer<boolList>& fm,
  const label index,
  const faceZoneMesh& zm
)
:
  zone{fz, addr, index},
  flipMap_{fm},
  zoneMesh_{zm},
  patchPtr_{NULL},
  masterCellsPtr_{NULL},
  slaveCellsPtr_{NULL},
  mePtr_{NULL}
{
  checkAddressing();
}


// Destructor
mousse::faceZone::~faceZone()
{
  clearAddressing();
}


// Member Functions
const mousse::faceZoneMesh& mousse::faceZone::zoneMesh() const
{
  return zoneMesh_;
}


mousse::label mousse::faceZone::whichFace(const label globalFaceID) const
{
  return zone::localID(globalFaceID);
}


const mousse::primitiveFacePatch& mousse::faceZone::operator()() const
{
  if (!patchPtr_) {
    calcFaceZonePatch();
  }
  return *patchPtr_;
}


const mousse::labelList& mousse::faceZone::masterCells() const
{
  if (!masterCellsPtr_) {
    calcCellLayers();
  }
  return *masterCellsPtr_;
}


const mousse::labelList& mousse::faceZone::slaveCells() const
{
  if (!slaveCellsPtr_) {
    calcCellLayers();
  }
  return *slaveCellsPtr_;
}


const mousse::labelList& mousse::faceZone::meshEdges() const
{
  if (!mePtr_) {
    mePtr_ =
      new labelList
      {
        operator()().meshEdges
        (
          zoneMesh().mesh().edges(),
          zoneMesh().mesh().pointEdges()
        )
      };
  }
  return *mePtr_;
}


void mousse::faceZone::clearAddressing()
{
  zone::clearAddressing();
  deleteDemandDrivenData(patchPtr_);
  deleteDemandDrivenData(masterCellsPtr_);
  deleteDemandDrivenData(slaveCellsPtr_);
  deleteDemandDrivenData(mePtr_);
}


void mousse::faceZone::resetAddressing
(
  const labelUList& addr,
  const boolList& flipMap
)
{
  clearAddressing();
  labelList::operator=(addr);
  flipMap_ = flipMap;
}


void mousse::faceZone::updateMesh(const mapPolyMesh& mpm)
{
  clearAddressing();
  labelList newAddressing{size()};
  boolList newFlipMap{flipMap_.size()};
  label nFaces = 0;
  const labelList& faceMap = mpm.reverseFaceMap();
  FOR_ALL(*this, i) {
    const label faceI = operator[](i);
    if (faceMap[faceI] >= 0) {
      newAddressing[nFaces] = faceMap[faceI];
      newFlipMap[nFaces] = flipMap_[i];       // Keep flip map.
      nFaces++;
    }
  }
  newAddressing.setSize(nFaces);
  newFlipMap.setSize(nFaces);
  transfer(newAddressing);
  flipMap_.transfer(newFlipMap);
}


bool mousse::faceZone::checkDefinition(const bool report) const
{
  return zone::checkDefinition(zoneMesh().mesh().faces().size(), report);
}


bool mousse::faceZone::checkParallelSync(const bool report) const
{
  const polyMesh& mesh = zoneMesh().mesh();
  const polyBoundaryMesh& bm = mesh.boundaryMesh();
  bool hasError = false;

  // Check that zone faces are synced
  {
    boolList neiZoneFace{mesh.nFaces()-mesh.nInternalFaces(), false};
    boolList neiZoneFlip{mesh.nFaces()-mesh.nInternalFaces(), false};
    FOR_ALL(*this, i) {
      const label faceI = operator[](i);
      if (!mesh.isInternalFace(faceI)) {
        neiZoneFace[faceI-mesh.nInternalFaces()] = true;
        neiZoneFlip[faceI-mesh.nInternalFaces()] = flipMap()[i];
      }
    }
    boolList myZoneFace{neiZoneFace};
    syncTools::swapBoundaryFaceList(mesh, neiZoneFace);
    boolList myZoneFlip{neiZoneFlip};
    syncTools::swapBoundaryFaceList(mesh, neiZoneFlip);
    FOR_ALL(*this, i) {
      const label faceI = operator[](i);
      const label patchI = bm.whichPatch(faceI);
      if (patchI != -1 && bm[patchI].coupled()) {
        const label bFaceI = faceI-mesh.nInternalFaces();
        // Check face in zone on both sides
        if (myZoneFace[bFaceI] != neiZoneFace[bFaceI]) {
          hasError = true;
          if (report) {
            Pout << " ***Problem with faceZone " << index()
              << " named " << name()
              << ". Face " << faceI
              << " on coupled patch "
              << bm[patchI].name()
              << " is not consistent with its coupled neighbour."
              << endl;
          } else {
            // w/o report - can stop checking now
            break;
          }
        } else if (myZoneFlip[bFaceI] == neiZoneFlip[bFaceI]) {
          // Flip state should be opposite.
          hasError = true;
          if (report) {
            Pout << " ***Problem with faceZone " << index()
              << " named " << name()
              << ". Face " << faceI
              << " on coupled patch "
              << bm[patchI].name()
              << " does not have consistent flipMap"
              << " across coupled faces."
              << endl;
          } else {
            // w/o report - can stop checking now
            break;
          }
        }
      }
    }
  }
  return returnReduce(hasError, orOp<bool>());
}


void mousse::faceZone::movePoints(const pointField& p)
{
  if (patchPtr_) {
    patchPtr_->movePoints(p);
  }
}


void mousse::faceZone::write(Ostream& os) const
{
  os << nl << name()
    << nl << static_cast<const labelList&>(*this)
    << nl << flipMap();
}


void mousse::faceZone::writeDict(Ostream& os) const
{
  os << nl << name() << nl << token::BEGIN_BLOCK << nl
    << "    type " << type() << token::END_STATEMENT << nl;
  writeEntry(this->labelsName, os);
  flipMap().writeEntry("flipMap", os);
  os << token::END_BLOCK << endl;
}


// Ostream Operator
mousse::Ostream& mousse::operator<<(Ostream& os, const faceZone& zn)
{
  zn.write(os);
  os.check("Ostream& operator<<(Ostream&, const faceZone&");
  return os;
}

