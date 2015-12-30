// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCentredCellToCellStencil
// Description
// SourceFiles
//   extended_centred_cell_to_cell_stencil.cpp
#ifndef extended_centred_cell_to_cell_stencil_hpp_
#define extended_centred_cell_to_cell_stencil_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    extendedCentredCellToCellStencil
    (
      const extendedCentredCellToCellStencil&
    );
    //- Disallow default bitwise assignment
    void operator=(const extendedCentredCellToCellStencil&);
public:
  // Constructors
    //- Construct from uncompacted cell stencil
    explicit extendedCentredCellToCellStencil(const cellToCellStencil&);
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
