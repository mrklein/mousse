#ifndef CORE_MESHES_POLY_MESH_POLY_PATCHES_SYMMETRY_PLANE_POLY_PATCH_HPP_
#define CORE_MESHES_POLY_MESH_POLY_PATCHES_SYMMETRY_PLANE_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlanePolyPatch
// Description
//   Symmetry-plane patch.

#include "poly_patch.hpp"


namespace mousse {

class symmetryPlanePolyPatch
:
  public polyPatch
{
  // Private data
    //- Symmetry plane normal
    vector n_;
protected:
  // Protected Member Functions
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
public:
  //- Runtime type information
  TYPE_NAME("symmetryPlane");
  // Constructors
    //- Construct from components
    symmetryPlanePolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    symmetryPlanePolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    symmetryPlanePolyPatch
    (
      const symmetryPlanePolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    symmetryPlanePolyPatch
    (
      const symmetryPlanePolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    symmetryPlanePolyPatch
    (
      const symmetryPlanePolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new symmetryPlanePolyPatch(*this, bm));
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
        new symmetryPlanePolyPatch(*this, bm, index, newSize, newStart)
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
      {
        new symmetryPlanePolyPatch
        {
          *this,
          bm,
          index,
          mapAddressing,
          newStart
        }
      };
    }
  // Member Functions
    //- Return symmetry plane normal
    const vector& n() const
    {
      return n_;
    }
};

}  // namespace mousse

#endif
