#ifndef MESH_TOOLS_MAPPED_PATCHES_MAPPED_POLY_PATCH_MAPPED_VARIABLE_THICKNESS_WALL_POLY_PATCH_HPP_
#define MESH_TOOLS_MAPPED_PATCHES_MAPPED_POLY_PATCH_MAPPED_VARIABLE_THICKNESS_WALL_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedVariableThicknessWallPolyPatch
// Description
//   mousse::mappedVariableThicknessWallPolyPatch
// SourceFiles
//   mapped_variable_thickness_wall_poly_patch.cpp
#include "wall_poly_patch.hpp"
#include "mapped_wall_poly_patch.hpp"
namespace mousse
{
class polyMesh;
class mappedVariableThicknessWallPolyPatch
:
  public mappedWallPolyPatch
{
  // Private data
    //- Thickness
    scalarList thickness_;
public:
  //- Runtime type information
  TYPE_NAME("mappedWallVariableThickness");
  // Constructors
    //- Construct from components
    mappedVariableThicknessWallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from components
    mappedVariableThicknessWallPolyPatch
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
    mappedVariableThicknessWallPolyPatch
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
    mappedVariableThicknessWallPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    mappedVariableThicknessWallPolyPatch
    (
      const mappedVariableThicknessWallPolyPatch&,
      const polyBoundaryMesh&
    );
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    mappedVariableThicknessWallPolyPatch
    (
      const mappedVariableThicknessWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    mappedVariableThicknessWallPolyPatch
    (
      const mappedVariableThicknessWallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>
      (
        new mappedVariableThicknessWallPolyPatch(*this, bm)
      );
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
        new mappedVariableThicknessWallPolyPatch
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
        new mappedVariableThicknessWallPolyPatch
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
  virtual ~mappedVariableThicknessWallPolyPatch();
  // Member functions
    //- Return non const thickness
    scalarList& thickness()
    {
      return thickness_;
    }
    //- Return const thickness
    const scalarList& thickness() const
    {
      return thickness_;
    }
    //- Write the polyPatch data as a dictionary
    void write(Ostream&) const;
};
}  // namespace mousse
#endif
