// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "sub_field.hpp"
#include "transform.hpp"
#include "pstream_reduce_ops.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(wedgePolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, wedgePolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, wedgePolyPatch, dictionary);

}

// Private Member Functions
void mousse::wedgePolyPatch::calcGeometry(PstreamBuffers&)
{
  if (axis_ != vector::rootMax)
  {
    return;
  }
  if (returnReduce(size(), sumOp<label>()))
  {
    const vectorField& nf(faceNormals());
    n_ = gAverage(nf);
    if (debug)
    {
      Info<< "Patch " << name() << " calculated average normal "
        << n_ << endl;
    }
    // Check the wedge is planar
    FOR_ALL(nf, faceI)
    {
      if (magSqr(n_ - nf[faceI]) > SMALL)
      {
        // only issue warning instead of error so that the case can
        // still be read for post-processing
        WARNING_IN
        (
          "wedgePolyPatch::calcGeometry(PstreamBuffers&)"
        )
        << "Wedge patch '" << name() << "' is not planar." << nl
        << "At local face at "
        << primitivePatch::faceCentres()[faceI]
        << " the normal " << nf[faceI]
        << " differs from the average normal " << n_
        << " by " << magSqr(n_ - nf[faceI]) << nl
        << "Either correct the patch or split it into planar parts"
        << endl;
      }
    }
    centreNormal_ =
      vector
      (
        sign(n_.x())*(max(mag(n_.x()), 0.5) - 0.5),
        sign(n_.y())*(max(mag(n_.y()), 0.5) - 0.5),
        sign(n_.z())*(max(mag(n_.z()), 0.5) - 0.5)
      );
    centreNormal_ /= mag(centreNormal_);
    cosAngle_ = centreNormal_ & n_;
    const scalar cnCmptSum =
      centreNormal_.x() + centreNormal_.y() + centreNormal_.z();
    if (mag(cnCmptSum) < (1 - SMALL))
    {
      FATAL_ERROR_IN("wedgePolyPatch::calcGeometry(PstreamBuffers&)")
        << "wedge " << name()
        << " centre plane does not align with a coordinate plane by "
        << 1 - mag(cnCmptSum)
        << exit(FatalError);
    }
    axis_ = centreNormal_ ^ n_;
    scalar magAxis = mag(axis_);
    if (magAxis < SMALL)
    {
      FATAL_ERROR_IN("wedgePolyPatch::calcGeometry(PstreamBuffers&)")
        << "wedge " << name()
        << " plane aligns with a coordinate plane." << nl
        << "    The wedge plane should make a small angle (~2.5deg)"
         " with the coordinate plane" << nl
        << "    and the the pair of wedge planes should be symmetric"
        << " about the coordinate plane." << nl
        << "    Normal of wedge plane is " << n_
        << " , implied coordinate plane direction is " << centreNormal_
        << exit(FatalError);
    }
    axis_ /= magAxis;
    faceT_ = rotationTensor(centreNormal_, n_);
    cellT_ = faceT_ & faceT_;
  }
}

// Constructors
mousse::wedgePolyPatch::wedgePolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, size, start, index, bm, patchType),
  axis_(vector::rootMax),
  centreNormal_(vector::rootMax),
  n_(vector::rootMax),
  cosAngle_(0.0),
  faceT_(tensor::zero),
  cellT_(tensor::zero)
{}
mousse::wedgePolyPatch::wedgePolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, dict, index, bm, patchType),
  axis_(vector::rootMax),
  centreNormal_(vector::rootMax),
  n_(vector::rootMax),
  cosAngle_(0.0),
  faceT_(tensor::zero),
  cellT_(tensor::zero)
{}
mousse::wedgePolyPatch::wedgePolyPatch
(
  const wedgePolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm),
  axis_(pp.axis_),
  centreNormal_(pp.centreNormal_),
  n_(pp.n_),
  cosAngle_(pp.cosAngle_),
  faceT_(pp.faceT_),
  cellT_(pp.cellT_)
{}
mousse::wedgePolyPatch::wedgePolyPatch
(
  const wedgePolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart),
  axis_(pp.axis_),
  centreNormal_(pp.centreNormal_),
  n_(pp.n_),
  cosAngle_(pp.cosAngle_),
  faceT_(pp.faceT_),
  cellT_(pp.cellT_)
{}
mousse::wedgePolyPatch::wedgePolyPatch
(
  const wedgePolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart),
  axis_(pp.axis_),
  centreNormal_(pp.centreNormal_),
  n_(pp.n_),
  cosAngle_(pp.cosAngle_),
  faceT_(pp.faceT_),
  cellT_(pp.cellT_)
{}
