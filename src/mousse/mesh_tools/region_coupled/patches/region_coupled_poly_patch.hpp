#ifndef MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_POLY_PATCH_REGION_COUPLED_POLY_PATCH_HPP_
#define MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_POLY_PATCH_REGION_COUPLED_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledPolyPatch
// Description
//   Region coupled polyPatch

#include "region_coupled_base.hpp"
#include "poly_boundary_mesh.hpp"
#include "poly_patch.hpp"


namespace mousse {

class regionCoupledPolyPatch
:
  public polyPatch,
  public regionCoupledBase
{
protected:
  // Protected Member Functions
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&);
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers& pBufs, const pointField&);
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers& pBufs, const pointField&);
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&);
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&);
public:
  //- Runtime type information
  TYPE_NAME("regionCoupled");
  // Constructors
    //- Construct from (base couped patch) components
    regionCoupledPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    regionCoupledPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    regionCoupledPolyPatch
    (
      const regionCoupledPolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    regionCoupledPolyPatch
    (
      const regionCoupledPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    regionCoupledPolyPatch
    (
      const regionCoupledPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new regionCoupledPolyPatch(*this, bm));
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
      {
        new regionCoupledPolyPatch
        {
          *this,
          bm,
          index,
          newSize,
          newStart
        }
      };
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
        new regionCoupledPolyPatch
        {
          *this,
          bm,
          index,
          mapAddressing,
          newStart
        }
      };
    }
  //- Destructor
  virtual ~regionCoupledPolyPatch();
  // Member functions
    // Access
      //- Is it coupled?
      virtual bool coupled() const
      {
        return false;
      }
    //- Return the type
    virtual const word& regionCoupleType() const
    {
      return type();
    }
    //- Write the polyPatch data as a dictionary
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
