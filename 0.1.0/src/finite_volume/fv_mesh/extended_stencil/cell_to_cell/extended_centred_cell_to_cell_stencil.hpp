#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_EXTENDED_CENTRED_CELL_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_EXTENDED_CENTRED_CELL_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCentredCellToCellStencil
// Description
// SourceFiles
//   extended_centred_cell_to_cell_stencil.cpp
#include "extended_cell_to_cell_stencil.hpp"
#include "extended_cell_to_face_stencil.hpp"
namespace mousse
{
class cellToCellStencil;
class extendedCentredCellToCellStencil
:
  public extendedCellToCellStencil
{
  // Private data
    //- Swap map for getting neigbouring data
    autoPtr<mapDistribute> mapPtr_;
    //- Per cell the stencil.
    labelListList stencil_;
public:
  // Constructors
    //- Construct from uncompacted cell stencil
    explicit extendedCentredCellToCellStencil(const cellToCellStencil&);
    //- Disallow default bitwise copy construct
    extendedCentredCellToCellStencil
    (
      const extendedCentredCellToCellStencil&
    ) = delete;
    //- Disallow default bitwise assignment
    extendedCentredCellToCellStencil& operator=
    (
      const extendedCentredCellToCellStencil&
    ) = delete;
  // Member Functions
    //- Return reference to the parallel distribution map
    const mapDistribute& map() const
    {
      return mapPtr_();
    }
    //- Return reference to the stencil
    const labelListList& stencil() const
    {
      return stencil_;
    }
    //- After removing elements from the stencil adapt the schedule (map).
    void compact();
    //- Use map to get the data into stencil order
    template<class Type>
    void collectData
    (
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      List<List<Type> >& stencilFld
    ) const
    {
      extendedCellToFaceStencil::collectData
      (
        map(),
        stencil(),
        fld,
        stencilFld
      );
    }
    //- Sum vol field contributions to create cell values
    template<class Type, class WeightType>
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
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      const List<List<WeightType> >& stencilWeights
    ) const
    {
      return extendedCellToCellStencil::weightedSum
      (
        map(),
        stencil(),
        fld,
        stencilWeights
      );
    }
};
}  // namespace mousse
#endif
