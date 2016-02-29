#ifndef CORE_MESHES_POLY_MESH_POLY_PATCHES_WEDGE_POLY_PATCH_HPP_
#define CORE_MESHES_POLY_MESH_POLY_PATCHES_WEDGE_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgePolyPatch
// Description
//   Wedge front and back plane patch.
// SourceFiles
//   wedge_poly_patch.cpp
#include "poly_patch.hpp"
namespace mousse
{
class wedgePolyPatch
:
  public polyPatch
{
  // Private data
    //- Axis of the wedge
    vector axis_;
    //- Centre normal between the wedge boundaries
    vector centreNormal_;
    //- Normal to the patch
    vector n_;
    //- Cosine of the wedge angle
    scalar cosAngle_;
    //- Face transformation tensor
    tensor faceT_;
    //- Neighbour-cell transformation tensor
    tensor cellT_;
protected:
  // Protected Member Functions
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
public:
  //- Runtime type information
  TYPE_NAME("wedge");
  // Constructors
    //- Construct from components
    wedgePolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    wedgePolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    wedgePolyPatch(const wedgePolyPatch&, const polyBoundaryMesh&);
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    wedgePolyPatch
    (
      const wedgePolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    wedgePolyPatch
    (
      const wedgePolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new wedgePolyPatch(*this, bm));
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
        new wedgePolyPatch(*this, bm, index, newSize, newStart)
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
        new wedgePolyPatch(*this, bm, index, mapAddressing, newStart)
      );
    }
  // Member functions
    // Access
      //- Return axis of the wedge
      const vector& axis() const
      {
        return axis_;
      }
      //- Return plane normal between the wedge boundaries
      const vector& centreNormal() const
      {
        return centreNormal_;
      }
      //- Return the normal to the patch
      const vector& n() const
      {
        return n_;
      }
      //- Return the cosine of the wedge angle
      scalar cosAngle() const
      {
        return cosAngle_;
      }
      //- Return face transformation tensor
      const tensor& faceT() const
      {
        return faceT_;
      }
      //- Return neighbour-cell transformation tensor
      const tensor& cellT() const
      {
        return cellT_;
      }
};
}  // namespace mousse
#endif
