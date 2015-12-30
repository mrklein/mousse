// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedWallPolyPatch
// Description
//   Determines a mapping between patch face centres and mesh cell or face
//   centres and processors they're on.
// Note
//   Storage is not optimal. It stores all face centres and cells on all
//   processors to keep the addressing calculation simple.
// SourceFiles
//   mapped_wall_poly_patch.cpp
#ifndef mapped_wall_poly_patch_hpp_
#define mapped_wall_poly_patch_hpp_
#include "wall_poly_patch.hpp"
#include "mapped_patch_base.hpp"
namespace mousse
{
class polyMesh;
class mappedWallPolyPatch
:
  public wallPolyPatch,
  public mappedPatchBase
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
  TypeName("mappedWall");
  // Constructors
    //- Construct from components
    mappedWallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from components
    mappedWallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const word& sampleRegion,
      const mappedPatchBase::sampleMode mode,
      const word& samplePatch,
      const vectorField& offset,
      const polyBoundaryMesh& bm
    );
    //- Construct from components. Uniform offset.
    mappedWallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const word& sampleRegion,
      const mappedPatchBase::sampleMode mode,
      const word& samplePatch,
      const vector& offset,
      const polyBoundaryMesh& bm
    );
    //- Construct from dictionary
    mappedWallPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    mappedWallPolyPatch
    (
      const mappedWallPolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    mappedWallPolyPatch
    (
      const mappedWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    mappedWallPolyPatch
    (
      const mappedWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new mappedWallPolyPatch(*this, bm));
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
        new mappedWallPolyPatch
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
        new mappedWallPolyPatch
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
  virtual ~mappedWallPolyPatch();
  // Member functions
    //- Write the polyPatch data as a dictionary
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
