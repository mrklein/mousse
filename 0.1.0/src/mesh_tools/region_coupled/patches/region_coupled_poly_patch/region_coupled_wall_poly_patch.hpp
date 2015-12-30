// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledWallPolyPatch
// Description
//   regionCoupledWall
// SourceFiles
//   region_coupled_wall_poly_patch.cpp
#ifndef region_coupled_wall_poly_patch_hpp_
#define region_coupled_wall_poly_patch_hpp_
#include "wall_poly_patch.hpp"
#include "region_coupled_base.hpp"
namespace mousse
{
class polyMesh;
class regionCoupledWallPolyPatch
:
  public wallPolyPatch,
  public regionCoupledBase
{
protected:
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&);
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers&, const pointField&);
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers&, const pointField&);
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&);
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&);
public:
  //- Runtime type information
  TypeName("regionCoupledWall");
  // Constructors
    //- Construct from components
    regionCoupledWallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    regionCoupledWallPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    regionCoupledWallPolyPatch
    (
      const regionCoupledWallPolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    regionCoupledWallPolyPatch
    (
      const regionCoupledWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    regionCoupledWallPolyPatch
    (
      const regionCoupledWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<wallPolyPatch::polyPatch> clone
    (
      const polyBoundaryMesh& bm
    ) const
    {
      return autoPtr<wallPolyPatch::polyPatch>
      (
        new regionCoupledWallPolyPatch(*this, bm)
      );
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<wallPolyPatch::polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    ) const
    {
      return autoPtr<wallPolyPatch::polyPatch>
      (
        new regionCoupledWallPolyPatch
        (
          *this,
          bm,
          index,
          newSize,
          newStart
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
        new regionCoupledWallPolyPatch
        (
          *this,
          bm,
          index,
          mapAddressing,
          newStart
        )
      );
    }
  //- Destructor
  virtual ~regionCoupledWallPolyPatch();
  // Member functions
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
