#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_EXTENDED_CELL_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_EXTENDED_CELL_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCellToCellStencil
// Description
//   Baseclass for cell-to-cell stencils
// SourceFiles
//   extended_cell_to_cell_stencil.cpp
//   extended_cell_to_cell_stencil_templates.cpp
#include "map_distribute.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class globalIndex;
class extendedCellToCellStencil
{
protected:
  // Protected data
    const polyMesh& mesh_;
public:
  // Constructors
    //- Construct from mesh
    explicit extendedCellToCellStencil(const polyMesh&);
  // Member Functions
    //- Sum surface field contributions to create cell values
    template<class Type, class WeightType>
    static
    tmp
    <
      GeometricField
      <
        typename outerProduct<WeightType, Type>::type,
        fvPatchField,
        volMesh
      >
    > weightedSum
    (
      const mapDistribute& map,
      const labelListList& stencil,
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      const List<List<WeightType> >& stencilWeights
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "extended_cell_to_cell_stencil_templates.cpp"
#endif
#endif
