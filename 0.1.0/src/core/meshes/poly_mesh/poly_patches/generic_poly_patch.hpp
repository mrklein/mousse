#ifndef CORE_MESHES_POLY_MESH_POLY_PATCHES_GENERIC_POLY_PATCH_HPP_
#define CORE_MESHES_POLY_MESH_POLY_PATCHES_GENERIC_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericPolyPatch
// Description
//   Substitute for unknown patches. Used for postprocessing when only
//   basic polyPatch info is needed.
// Note
//   Storage is not optimal. It stores all face centres and cells on all
//   processors to keep the addressing calculation simple.
// SourceFiles
//   generic_poly_patch.cpp
#include "poly_patch.hpp"
#include "dictionary.hpp"
namespace mousse
{
class genericPolyPatch
:
  public polyPatch
{
  // Private data
    word actualTypeName_;
    dictionary dict_;
public:
  //- Runtime type information
  TYPE_NAME("genericPatch");
  // Constructors
    //- Construct from components
    genericPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    genericPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    genericPolyPatch
    (
      const genericPolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    genericPolyPatch
    (
      const genericPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    genericPolyPatch
    (
      const genericPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new genericPolyPatch(*this, bm));
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
        new genericPolyPatch(*this, bm, index, newSize, newStart)
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
        new genericPolyPatch(*this, bm, index, mapAddressing, newStart)
      );
    }
  //- Destructor
  ~genericPolyPatch();
  // Member functions
    //- Write the polyPatch data as a dictionary
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
