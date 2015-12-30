// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_cell_to_face_stencil.hpp"
#include "global_index.hpp"
#include "sync_tools.hpp"
#include "sortable_list.hpp"
/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */
namespace mousse
{
defineTypeNameAndDebug(extendedCellToFaceStencil, 0);
}
// Private Member Functions 
void mousse::extendedCellToFaceStencil::writeStencilStats
(
  Ostream& os,
  const labelListList& stencil,
  const mapDistribute& map
)
{
  label sumSize = 0;
  label nSum = 0;
  label minSize = labelMax;
  label maxSize = labelMin;
  forAll(stencil, i)
  {
    const labelList& sCells = stencil[i];
    if (sCells.size() > 0)
    {
      sumSize += sCells.size();
      nSum++;
      minSize = min(minSize, sCells.size());
      maxSize = max(maxSize, sCells.size());
    }
  }
  reduce(sumSize, sumOp<label>());
  reduce(nSum, sumOp<label>());
  reduce(minSize, minOp<label>());
  reduce(maxSize, maxOp<label>());
  os  << "Stencil size :" << nl
    << "    average : " << scalar(sumSize)/nSum << nl
    << "    min     : " << minSize << nl
    << "    max     : " << maxSize << nl
    << endl;
  // Sum all sent data
  label nSent = 0;
  label nLocal = 0;
  forAll(map.subMap(), procI)
  {
    if (procI != Pstream::myProcNo())
    {
      nSent += map.subMap()[procI].size();
    }
    else
    {
      nLocal += map.subMap()[procI].size();
    }
  }
  os  << "Local data size : " << returnReduce(nLocal, sumOp<label>()) << nl
    << "Sent data size  : " << returnReduce(nSent, sumOp<label>()) << nl
    << endl;
}
// Constructors 
mousse::extendedCellToFaceStencil::extendedCellToFaceStencil(const polyMesh& mesh)
:
  mesh_(mesh)
{
  // Check for transformation - not supported.
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  forAll(patches, patchI)
  {
    if (patches[patchI].coupled())
    {
      const coupledPolyPatch& cpp =
        refCast<const coupledPolyPatch>(patches[patchI]);
      if (!cpp.parallel() || cpp.separated())
      {
        FatalErrorIn
        (
          "extendedCellToFaceStencil::extendedCellToFaceStencil"
          "(const polyMesh&)"
        )   << "Coupled patches with transformations not supported."
          << endl
          << "Problematic patch " << cpp.name() << exit(FatalError);
      }
    }
  }
}
