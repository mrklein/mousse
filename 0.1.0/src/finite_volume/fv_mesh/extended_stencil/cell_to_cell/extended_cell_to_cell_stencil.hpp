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
#ifndef extended_cell_to_cell_stencil_hpp_
#define extended_cell_to_cell_stencil_hpp_
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
