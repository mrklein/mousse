// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicSlipPolyPatch
// Description
//   Copy of cyclicSlip - used to be able to instantiate cyclicSlip pointPatch
//   which is cyclicSlip with slip constraints
// SourceFiles
//   cyclic_slip_poly_patch.cpp
#ifndef cyclic_slip_poly_patch_hpp_
#define cyclic_slip_poly_patch_hpp_
#include "cyclic_poly_patch.hpp"
namespace mousse
{
class cyclicSlipPolyPatch
:
  public cyclicPolyPatch
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicSlip");
  // Constructors
    //- Construct from components
    cyclicSlipPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType,
      const transformType transform = UNKNOWN
    )
    :
      cyclicPolyPatch(name, size, start, index, bm, patchType, transform)
    {}
    //- Construct from dictionary
    cyclicSlipPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    )
    :
      cyclicPolyPatch(name, dict, index, bm, patchType)
    {}
    //- Construct as copy, resetting the boundary mesh
    cyclicSlipPolyPatch
    (
      const cyclicSlipPolyPatch& pp,
      const polyBoundaryMesh& bm
    )
    :
      cyclicPolyPatch(pp, bm)
    {}
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    cyclicSlipPolyPatch
    (
      const cyclicSlipPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart,
      const word& neighbPatchName
    )
    :
      cyclicPolyPatch(pp, bm, index, newSize, newStart, neighbPatchName)
    {}
    //- Construct given the original patch and a map
    cyclicSlipPolyPatch
    (
      const cyclicPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    )
    :
      cyclicPolyPatch(pp, bm, index, mapAddressing, newStart)
    {}
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new cyclicSlipPolyPatch(*this, bm));
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      (
        new cyclicSlipPolyPatch
        (
          *this,
          bm,
          index,
          newSize,
          newStart,
          neighbPatchName()
        )
      );
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      (
        new cyclicSlipPolyPatch
        (
          *this,
          bm,
          index,
          mapAddressing,
          newStart
        )
      );
    }
  // Destructor
    virtual ~cyclicSlipPolyPatch()
    {}
};
}  // namespace mousse
#endif
