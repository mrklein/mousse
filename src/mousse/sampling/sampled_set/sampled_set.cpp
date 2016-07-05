// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_set.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "mesh_search.hpp"
#include "writer.hpp"
#include "particle.hpp"


// Static Data Members
namespace mousse {

const scalar sampledSet::tol = 1e-6;
DEFINE_TYPE_NAME_AND_DEBUG(sampledSet, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(sampledSet, word);

}


// Private Member Functions 
mousse::label mousse::sampledSet::getBoundaryCell(const label faceI) const
{
  return mesh().faceOwner()[faceI];
}


mousse::label mousse::sampledSet::getCell
(
  const label faceI,
  const point& sample
) const
{
  if (faceI == -1) {
    FATAL_ERROR_IN
    (
      "sampledSet::getCell(const label, const point&)"
    )
    << "Illegal face label " << faceI
    << abort(FatalError);
  }
  if (faceI >= mesh().nInternalFaces()) {
    label cellI = getBoundaryCell(faceI);
    if (!mesh().pointInCell(sample, cellI, searchEngine_.decompMode())) {
      FATAL_ERROR_IN
      (
        "sampledSet::getCell(const label, const point&)"
      )
      << "Found cell " << cellI << " using face " << faceI
      << ". But cell does not contain point " << sample
      << abort(FatalError);
    }
    return cellI;
  } else {
    // Try owner and neighbour to see which one contains sample
    label cellI = mesh().faceOwner()[faceI];
    if (mesh().pointInCell(sample, cellI, searchEngine_.decompMode())) {
      return cellI;
    } else {
      cellI = mesh().faceNeighbour()[faceI];
      if (mesh().pointInCell(sample, cellI, searchEngine_.decompMode())) {
        return cellI;
      } else {
        FATAL_ERROR_IN
        (
          "sampledSet::getCell(const label, const point&)"
        )
        << "None of the neighbours of face "
        << faceI << " contains point " << sample
        << abort(FatalError);
        return -1;
      }
    }
  }
}


mousse::scalar mousse::sampledSet::calcSign
(
  const label faceI,
  const point& sample
) const
{
  vector vec = sample - mesh().faceCentres()[faceI];
  scalar magVec = mag(vec);
  if (magVec < VSMALL) {
    // sample on face centre. Regard as inside
    return -1;
  }
  vec /= magVec;
  vector n = mesh().faceAreas()[faceI];
  n /= mag(n) + VSMALL;
  return n & vec;
}


// Return face (or -1) of face which is within smallDist of sample
mousse::label mousse::sampledSet::findNearFace
(
  const label cellI,
  const point& sample,
  const scalar smallDist
) const
{
  const cell& myFaces = mesh().cells()[cellI];
  FOR_ALL(myFaces, myFaceI) {
    const face& f = mesh().faces()[myFaces[myFaceI]];
    pointHit inter = f.nearestPoint(sample, mesh().points());
    scalar dist;
    if (inter.hit()) {
      dist = mag(inter.hitPoint() - sample);
    } else {
      dist = mag(inter.missPoint() - sample);
    }
    if (dist < smallDist) {
      return myFaces[myFaceI];
    }
  }
  return -1;
}


// 'Pushes' point facePt (which is almost on face) in direction of cell centre
// so it is clearly inside.
mousse::point mousse::sampledSet::pushIn
(
  const point& facePt,
  const label faceI
) const
{
  label cellI = mesh().faceOwner()[faceI];
  const point& cC = mesh().cellCentres()[cellI];
  point newPosition = facePt;
  // Taken from particle::initCellFacePt()
  label tetFaceI;
  label tetPtI;
  mesh().findTetFacePt(cellI, facePt, tetFaceI, tetPtI);
  if (tetFaceI == -1 || tetPtI == -1) {
    newPosition = facePt;
    label trap(1.0/particle::trackingCorrectionTol + 1);
    label iterNo = 0;
    do {
      newPosition += particle::trackingCorrectionTol*(cC - facePt);
      mesh().findTetFacePt(cellI, newPosition, tetFaceI, tetPtI);
      iterNo++;
    } while (tetFaceI < 0  && iterNo <= trap);
  }
  if (tetFaceI == -1) {
    FATAL_ERROR_IN
    (
      "sampledSet::pushIn(const point&, const label)"
    )
    << "After pushing " << facePt << " to " << newPosition
    << " it is still outside face " << faceI
    << " at " << mesh().faceCentres()[faceI]
    << " of cell " << cellI
    << " at " << cC << endl
    << "Please change your starting point"
    << abort(FatalError);
  }
  return newPosition;
}


// Calculates start of tracking given samplePt and first boundary intersection
// (bPoint, bFaceI). bFaceI == -1 if no boundary intersection.
// Returns true if trackPt is sampling point
bool mousse::sampledSet::getTrackingPoint
(
  const vector& offset,
  const point& samplePt,
  const point& bPoint,
  const label bFaceI,
  point& trackPt,
  label& trackCellI,
  label& trackFaceI
) const
{
  const scalar smallDist = mag(tol*offset);
  bool isGoodSample = false;
  if (bFaceI == -1) {
    // No boundary intersection. Try and find cell samplePt is in
    trackCellI = mesh().findCell(samplePt, searchEngine_.decompMode());
    if ((trackCellI == -1) || !mesh().pointInCell(samplePt, trackCellI,
                                                  searchEngine_.decompMode())) {
      // Line samplePt - end_ does not intersect domain at all.
      // (or is along edge)
      trackCellI = -1;
      trackFaceI = -1;
      isGoodSample = false;
    } else {
      // start is inside. Use it as tracking point
      trackPt = samplePt;
      trackFaceI = -1;
      isGoodSample = true;
    }
  } else if (mag(samplePt - bPoint) < smallDist) {
    // samplePt close to bPoint. Snap to it
    trackPt = pushIn(bPoint, bFaceI);
    trackFaceI = bFaceI;
    trackCellI = getBoundaryCell(trackFaceI);
    isGoodSample = true;
  } else {
    scalar sign = calcSign(bFaceI, samplePt);
    if (sign < 0) {
      // samplePt inside or marginally outside.
      trackPt = samplePt;
      trackFaceI = -1;
      trackCellI = mesh().findCell(trackPt, searchEngine_.decompMode());
      isGoodSample = true;
    } else {
      // samplePt outside. use bPoint
      trackPt = pushIn(bPoint, bFaceI);
      trackFaceI = bFaceI;
      trackCellI = getBoundaryCell(trackFaceI);
      isGoodSample = false;
    }
  }
  if (debug) {
    Info << "sampledSet::getTrackingPoint :"
      << " offset:" << offset
      << " samplePt:" << samplePt
      << " bPoint:" << bPoint
      << " bFaceI:" << bFaceI
      << endl << "   Calculated first tracking point :"
      << " trackPt:" << trackPt
      << " trackCellI:" << trackCellI
      << " trackFaceI:" << trackFaceI
      << " isGoodSample:" << isGoodSample
      << endl;
  }
  return isGoodSample;
}


void mousse::sampledSet::setSamples
(
  const List<point>& samplingPts,
  const labelList& samplingCells,
  const labelList& samplingFaces,
  const labelList& samplingSegments,
  const scalarList& samplingCurveDist
)
{
  setSize(samplingPts.size());
  cells_.setSize(samplingCells.size());
  faces_.setSize(samplingFaces.size());
  segments_.setSize(samplingSegments.size());
  curveDist_.setSize(samplingCurveDist.size());
  if ((cells_.size() != size()) || (faces_.size() != size())
      || (segments_.size() != size()) || (curveDist_.size() != size())) {
    FATAL_ERROR_IN("sampledSet::setSamples()")
      << "sizes not equal : "
      << "  points:" << size()
      << "  cells:" << cells_.size()
      << "  faces:" << faces_.size()
      << "  segments:" << segments_.size()
      << "  curveDist:" << curveDist_.size()
      << abort(FatalError);
  }
  FOR_ALL(samplingPts, sampleI) {
    operator[](sampleI) = samplingPts[sampleI];
  }
  curveDist_ = samplingCurveDist;
  cells_ = samplingCells;
  faces_ = samplingFaces;
  segments_ = samplingSegments;
}


// Constructors 
mousse::sampledSet::sampledSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis
)
:
  coordSet{name, axis},
  mesh_{mesh},
  searchEngine_{searchEngine},
  segments_{0},
  cells_{0},
  faces_{0}
{}


mousse::sampledSet::sampledSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  coordSet{name, dict.lookup("axis")},
  mesh_{mesh},
  searchEngine_{searchEngine},
  segments_{0},
  cells_{0},
  faces_{0}
{}


// Destructor 
mousse::sampledSet::~sampledSet()
{}


// Member Functions 
mousse::autoPtr<mousse::sampledSet> mousse::sampledSet::New
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
{
  const word sampleType{dict.lookup("type")};
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(sampleType);
  if (cstrIter == wordConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "sampledSet::New"
      "(const word&, const polyMesh&, const meshSearch&"
      ", const dictionary&)"
    )
    << "Unknown sample type "
    << sampleType << nl << nl
    << "Valid sample types : " << endl
    << wordConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<sampledSet>{cstrIter()(name, mesh, searchEngine, dict)};
}


mousse::Ostream& mousse::sampledSet::write(Ostream& os) const
{
  coordSet::write(os);
  os << endl << "\t(cellI)\t(faceI)" << endl;
  FOR_ALL(*this, sampleI) {
    os << '\t' << cells_[sampleI] << '\t' << faces_[sampleI] << endl;
  }
  return os;
}

