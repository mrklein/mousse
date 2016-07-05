#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_EXTENDED_UPWIND_CELL_TO_FACE_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_EXTENDED_UPWIND_CELL_TO_FACE_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedUpwindCellToFaceStencil
// Description
//   Creates upwind stencil by shifting a centred stencil to upwind and downwind
//   faces and optionally removing all non-(up/down)wind faces ('pureUpwind').
//   Note: the minOpposedness parameter is to decide which upwind and
//   downwind faces to combine the stencils from. If myArea is the
//   local area and upwindArea
//   the area of the possible upwind candidate it will be included if
//     (upwindArea & myArea)/magSqr(myArea) > minOpposedness
//   so this includes both cosine and area. WIP.
// SourceFiles
//   extended_upwind_cell_to_face_stencil.cpp
//   extended_upwind_cell_to_face_stencil_templates.cpp
#include "extended_cell_to_face_stencil.hpp"
namespace mousse
{
class cellToFaceStencil;
class extendedUpwindCellToFaceStencil
:
  public extendedCellToFaceStencil
{
  // Private data
    //- Does stencil contain upwind points only
    const bool pureUpwind_;
    //- Swap map for getting neigbouring data
    autoPtr<mapDistribute> ownMapPtr_;
    autoPtr<mapDistribute> neiMapPtr_;
    //- Per face the stencil.
    labelListList ownStencil_;
    labelListList neiStencil_;
  // Private Member Functions
    //- Find most 'opposite' faces of cell
    void selectOppositeFaces
    (
      const boolList& nonEmptyFace,
      const scalar minOpposedness,
      const label faceI,
      const label cellI,
      DynamicList<label>& oppositeFaces
    ) const;
    //- Transport (centred) face stencil to 'opposite' face.
    void transportStencil
    (
      const boolList& nonEmptyFace,
      const labelListList& faceStencil,
      const scalar minOpposedness,
      const label faceI,
      const label cellI,
      const bool stencilHasNeighbour,
      DynamicList<label>& oppositeFaces,
      labelHashSet& faceStencilSet,
      labelList& transportedStencil
    ) const;
    //- Transport (centred) face stencil to 'opposite' faces.
    void transportStencils
    (
      const labelListList& faceStencil,
      const scalar minOpposedness,
      labelListList& ownStencil,
      labelListList& neiStencil
    );
public:
  // Constructors
    //- Construct from mesh and uncompacted centred face stencil.
    //  Transports facestencil to create owner and neighbour versions.
    //  pureUpwind to remove any remaining downwind cells.
    extendedUpwindCellToFaceStencil
    (
      const cellToFaceStencil&,
      const bool pureUpwind,
      const scalar minOpposedness
    );
    //- Construct from mesh and uncompacted centred face stencil. Splits
    //  stencil into owner and neighbour (so always pure upwind)
    extendedUpwindCellToFaceStencil
    (
      const cellToFaceStencil&
    );
    //- Disallow default bitwise copy construct
    extendedUpwindCellToFaceStencil
    (
      const extendedUpwindCellToFaceStencil&
    ) = delete;
    //- Disallow default bitwise assignment
    extendedUpwindCellToFaceStencil& operator=
    (
      const extendedUpwindCellToFaceStencil&
    ) = delete;
  // Member Functions
    bool pureUpwind() const
    {
      return pureUpwind_;
    }
    //- Return reference to the parallel distribution map
    const mapDistribute& ownMap() const
    {
      return ownMapPtr_();
    }
    //- Return reference to the parallel distribution map
    const mapDistribute& neiMap() const
    {
      return neiMapPtr_();
    }
    //- Return reference to the stencil
    const labelListList& ownStencil() const
    {
      return ownStencil_;
    }
    //- Return reference to the stencil
    const labelListList& neiStencil() const
    {
      return neiStencil_;
    }
    //- Sum vol field contributions to create face values
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > weightedSum
    (
      const surfaceScalarField& phi,
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      const List<List<scalar> >& ownWeights,
      const List<List<scalar> >& neiWeights
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "extended_upwind_cell_to_face_stencil_templates.cpp"
#endif
#endif
