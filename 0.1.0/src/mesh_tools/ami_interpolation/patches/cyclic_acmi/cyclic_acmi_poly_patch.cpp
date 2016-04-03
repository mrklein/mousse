// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_poly_patch.hpp"
#include "sub_field.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicACMIPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, cyclicACMIPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, cyclicACMIPolyPatch, dictionary);

}

const mousse::scalar mousse::cyclicACMIPolyPatch::tolerance_ = 1e-6;


// Protected Member Functions
void mousse::cyclicACMIPolyPatch::initPatchFaceAreas() const
{
  if (!empty() && (faceAreas0_.empty() || boundaryMesh().mesh().moving())) {
    faceAreas0_ = faceAreas();
  }
  const cyclicACMIPolyPatch& nbrACMI =
    refCast<const cyclicACMIPolyPatch>(this->neighbPatch());
  if (!nbrACMI.empty()
      && (nbrACMI.faceAreas0().empty() || boundaryMesh().mesh().moving())) {
    nbrACMI.faceAreas0_ = nbrACMI.faceAreas();
  }
}


void mousse::cyclicACMIPolyPatch::resetAMI
(
  const AMIPatchToPatchInterpolation::interpolationMethod&
) const
{
  if (!owner())
    return;
  const polyPatch& nonOverlapPatch = this->nonOverlapPatch();
  initPatchFaceAreas();
  // Reset patch face areas based on original patch for AMI calculation
  vectorField::subField Sf = faceAreas();
  vectorField::subField noSf = nonOverlapPatch.faceAreas();
  FOR_ALL(Sf, faceI) {
    Sf[faceI] = faceAreas0_[faceI];
    noSf[faceI] = faceAreas0_[faceI];
  }
  // Calculate the AMI using partial face-area-weighted
  cyclicAMIPolyPatch::resetAMI
  (
    AMIPatchToPatchInterpolation::imPartialFaceAreaWeight
  );
  srcMask_ =
    min(scalar(1) - tolerance_, max(tolerance_, AMI().srcWeightsSum()));
  tgtMask_ =
    min(scalar(1) - tolerance_, max(tolerance_, AMI().tgtWeightsSum()));
  FOR_ALL(Sf, faceI) {
    Sf[faceI] *= srcMask_[faceI];
    noSf[faceI] *= 1.0 - srcMask_[faceI];
  }
  setNeighbourFaceAreas();
  // Set the updated flag
  updated_ = true;
}


void mousse::cyclicACMIPolyPatch::setNeighbourFaceAreas() const
{
  const cyclicACMIPolyPatch& cp =
    refCast<const cyclicACMIPolyPatch>(this->neighbPatch());
  const polyPatch& pp = cp.nonOverlapPatch();
  const vectorField& faceAreas0 = cp.faceAreas0();
  if (tgtMask_.size() == cp.size()) {
    vectorField::subField Sf = cp.faceAreas();
    vectorField::subField noSf = pp.faceAreas();
    FOR_ALL(Sf, faceI) {
      Sf[faceI] = tgtMask_[faceI]*faceAreas0[faceI];
      noSf[faceI] = (1.0 - tgtMask_[faceI])*faceAreas0[faceI];
    }
  } else {
    WARNING_IN("cyclicACMIPolyPatch::setNeighbourFaceAreas() const")
      << "Target mask size differs to that of the neighbour patch\n"
      << "    May occur when decomposing." << endl;
  }
}


void mousse::cyclicACMIPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  cyclicAMIPolyPatch::initGeometry(pBufs);
  // Initialise the AMI
  resetAMI();
}


void mousse::cyclicACMIPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
  cyclicAMIPolyPatch::calcGeometry(pBufs);
}


void mousse::cyclicACMIPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  cyclicAMIPolyPatch::initMovePoints(pBufs, p);
  // Initialise the AMI
  resetAMI();
}


void mousse::cyclicACMIPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  cyclicAMIPolyPatch::movePoints(pBufs, p);
}


void mousse::cyclicACMIPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  cyclicAMIPolyPatch::initUpdateMesh(pBufs);
}


void mousse::cyclicACMIPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  cyclicAMIPolyPatch::updateMesh(pBufs);
}


void mousse::cyclicACMIPolyPatch::clearGeom()
{
  cyclicAMIPolyPatch::clearGeom();
}


const mousse::scalarField& mousse::cyclicACMIPolyPatch::srcMask() const
{
  return srcMask_;
}


const mousse::scalarField& mousse::cyclicACMIPolyPatch::tgtMask() const
{
  return tgtMask_;
}


// Constructors
mousse::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType,
  const transformType transform
)
:
  cyclicAMIPolyPatch{name, size, start, index, bm, patchType, transform},
  faceAreas0_{},
  nonOverlapPatchName_{word::null},
  nonOverlapPatchID_{-1},
  srcMask_{},
  tgtMask_{},
  updated_{false}
{
  AMIRequireMatch_ = false;
  // Non-overlapping patch might not be valid yet so cannot determine
  // associated patchID
}


mousse::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  cyclicAMIPolyPatch{name, dict, index, bm, patchType},
  faceAreas0_{},
  nonOverlapPatchName_{dict.lookup("nonOverlapPatch")},
  nonOverlapPatchID_{-1},
  srcMask_{},
  tgtMask_{},
  updated_{false}
{
  AMIRequireMatch_ = false;
  if (nonOverlapPatchName_ == name) {
    FATAL_IO_ERROR_IN
    (
      "cyclicACMIPolyPatch::cyclicACMIPolyPatch"
      "("
        "const word&, "
        "const dictionary&, "
        "const label, "
        "const polyBoundaryMesh&, "
        "const word&"
      ")",
      dict
    )
    << "Non-overlapping patch name " << nonOverlapPatchName_
    << " cannot be the same as this patch " << name
    << exit(FatalIOError);
  }
  // Non-overlapping patch might not be valid yet so cannot determine
  // associated patchID
}


mousse::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
  const cyclicACMIPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  cyclicAMIPolyPatch{pp, bm},
  faceAreas0_{},
  nonOverlapPatchName_{pp.nonOverlapPatchName_},
  nonOverlapPatchID_{-1},
  srcMask_{},
  tgtMask_{},
  updated_{false}
{
  AMIRequireMatch_ = false;
  // Non-overlapping patch might not be valid yet so cannot determine
  // associated patchID
}


mousse::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
  const cyclicACMIPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart,
  const word& nbrPatchName,
  const word& nonOverlapPatchName
)
:
  cyclicAMIPolyPatch{pp, bm, index, newSize, newStart, nbrPatchName},
  faceAreas0_{},
  nonOverlapPatchName_{nonOverlapPatchName},
  nonOverlapPatchID_{-1},
  srcMask_{},
  tgtMask_{},
  updated_{false}
{
  AMIRequireMatch_ = false;
  if (nonOverlapPatchName_ == name()) {
    FATAL_ERROR_IN
    (
      "const cyclicACMIPolyPatch& "
      "const polyBoundaryMesh&, "
      "const label, "
      "const label, "
      "const label, "
      "const word&, "
      "const word&"
    )
    << "Non-overlapping patch name " << nonOverlapPatchName_
    << " cannot be the same as this patch " << name()
    << exit(FatalError);
  }
  // Non-overlapping patch might not be valid yet so cannot determine
  // associated patchID
}


mousse::cyclicACMIPolyPatch::cyclicACMIPolyPatch
(
  const cyclicACMIPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  cyclicAMIPolyPatch{pp, bm, index, mapAddressing, newStart},
  faceAreas0_{},
  nonOverlapPatchName_{pp.nonOverlapPatchName_},
  nonOverlapPatchID_{-1},
  srcMask_{},
  tgtMask_{},
  updated_{false}
{
  AMIRequireMatch_ = false;
}


// Destructor
mousse::cyclicACMIPolyPatch::~cyclicACMIPolyPatch()
{}


// Member Functions
const mousse::cyclicACMIPolyPatch& mousse::cyclicACMIPolyPatch::neighbPatch() const
{
  const polyPatch& pp = this->boundaryMesh()[neighbPatchID()];
  return refCast<const cyclicACMIPolyPatch>(pp);
}


mousse::label mousse::cyclicACMIPolyPatch::nonOverlapPatchID() const
{
  if (nonOverlapPatchID_ == -1) {
    nonOverlapPatchID_ =
      this->boundaryMesh().findPatchID(nonOverlapPatchName_);
    if (nonOverlapPatchID_ == -1) {
      FATAL_ERROR_IN("cyclicPolyAMIPatch::neighbPatchID() const")
        << "Illegal non-overlapping patch name " << nonOverlapPatchName_
        << nl << "Valid patch names are "
        << this->boundaryMesh().names()
        << exit(FatalError);
    }
    if (nonOverlapPatchID_ < index()) {
      FATAL_ERROR_IN("cyclicPolyAMIPatch::neighbPatchID() const")
        << "Boundary ordering error: " << type()
        << " patch must be defined prior to its non-overlapping patch"
        << nl
        << type() << " patch: " << name() << ", ID:" << index() << nl
        << "Non-overlap patch: " << nonOverlapPatchName_
        << ", ID:" << nonOverlapPatchID_ << nl
        << exit(FatalError);
    }
    const polyPatch& noPp = this->boundaryMesh()[nonOverlapPatchID_];
    bool ok = true;
    if (size() == noPp.size()) {
      const scalarField magSf(mag(faceAreas()));
      const scalarField noMagSf(mag(noPp.faceAreas()));
      FOR_ALL(magSf, faceI) {
        scalar ratio = mag(magSf[faceI]/(noMagSf[faceI] + ROOTVSMALL));
        if (ratio - 1 > tolerance_) {
          ok = false;
          break;
        }
      }
    } else {
      ok = false;
    }
    if (!ok) {
      FATAL_ERROR_IN
      (
        "mousse::label "
        "mousse::cyclicACMIPolyPatch::nonOverlapPatchID() const"
      )
      << "Inconsistent ACMI patches " << name() << " and "
      << noPp.name() << ".  Patches should have identical topology"
      << exit(FatalError);
    }
  }
  return nonOverlapPatchID_;
}


void mousse::cyclicACMIPolyPatch::calcGeometry
(
  const primitivePatch& referPatch,
  const pointField& thisCtrs,
  const vectorField& thisAreas,
  const pointField& thisCc,
  const pointField& nbrCtrs,
  const vectorField& nbrAreas,
  const pointField& nbrCc
)
{
  cyclicAMIPolyPatch::calcGeometry
  (
    referPatch,
    thisCtrs,
    thisAreas,
    thisCc,
    nbrCtrs,
    nbrAreas,
    nbrCc
  );
}


void mousse::cyclicACMIPolyPatch::initOrder
(
  PstreamBuffers& pBufs,
  const primitivePatch& pp
) const
{
  cyclicAMIPolyPatch::initOrder(pBufs, pp);
}


bool mousse::cyclicACMIPolyPatch::order
(
  PstreamBuffers& pBufs,
  const primitivePatch& pp,
  labelList& faceMap,
  labelList& rotation
) const
{
  return cyclicAMIPolyPatch::order(pBufs, pp, faceMap, rotation);
}


void mousse::cyclicACMIPolyPatch::write(Ostream& os) const
{
  cyclicAMIPolyPatch::write(os);
  os.writeKeyword("nonOverlapPatch") << nonOverlapPatchName_
    << token::END_STATEMENT << nl;
}

