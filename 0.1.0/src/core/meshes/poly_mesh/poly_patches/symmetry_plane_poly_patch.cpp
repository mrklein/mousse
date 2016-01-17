// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "symmetry_poly_patch.hpp"
#include "pstream_reduce_ops.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(symmetryPlanePolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, symmetryPlanePolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, symmetryPlanePolyPatch, dictionary);

}

// Protected Member Functions
void mousse::symmetryPlanePolyPatch::calcGeometry(PstreamBuffers&)
{
  if (n_ == vector::rootMax)
  {
    if (returnReduce(size(), sumOp<label>()))
    {
      const vectorField& nf(faceNormals());
      n_ = gAverage(nf);
      if (debug)
      {
        Info<< "Patch " << name() << " calculated average normal "
          << n_ << endl;
      }
      // Check the symmetry plane is planar
      FOR_ALL(nf, facei)
      {
        if (magSqr(n_ - nf[facei]) > SMALL)
        {
          FATAL_ERROR_IN("symmetryPlanePolyPatch::n()")
            << "Symmetry plane '" << name() << "' is not planar."
            << endl
            << "At local face at "
            << primitivePatch::faceCentres()[facei]
            << " the normal " << nf[facei]
            << " differs from the average normal " << n_
            << " by " << magSqr(n_ - nf[facei]) << endl
            << "Either split the patch into planar parts"
            << " or use the " << symmetryPolyPatch::typeName
            << " patch type"
            << exit(FatalError);
        }
      }
    }
  }
}

// Constructors
mousse::symmetryPlanePolyPatch::symmetryPlanePolyPatch
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
  n_(vector::rootMax)
{}
mousse::symmetryPlanePolyPatch::symmetryPlanePolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, dict, index, bm, patchType),
  n_(vector::rootMax)
{}
mousse::symmetryPlanePolyPatch::symmetryPlanePolyPatch
(
  const symmetryPlanePolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm),
  n_(pp.n_)
{}
mousse::symmetryPlanePolyPatch::symmetryPlanePolyPatch
(
  const symmetryPlanePolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart),
  n_(pp.n_)
{}
mousse::symmetryPlanePolyPatch::symmetryPlanePolyPatch
(
  const symmetryPlanePolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart),
  n_(pp.n_)
{}
