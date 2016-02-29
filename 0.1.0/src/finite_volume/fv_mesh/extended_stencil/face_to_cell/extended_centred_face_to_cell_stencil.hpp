#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_FACE_TO_CELL_EXTENDED_CENTRED_FACE_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_FACE_TO_CELL_EXTENDED_CENTRED_FACE_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCentredFaceToCellStencil
// Description
// SourceFiles
//   extended_centred_face_to_cell_stencil.cpp
#include "extended_face_to_cell_stencil.hpp"
namespace mousse
{
class faceToCellStencil;
class extendedCentredFaceToCellStencil
:
  public extendedFaceToCellStencil
{
  // Private data
    //- Swap map for getting neigbouring data
    autoPtr<mapDistribute> mapPtr_;
    //- Per face the stencil.
    labelListList stencil_;
public:
  // Constructors
    //- Construct from uncompacted face stencil
    explicit extendedCentredFaceToCellStencil(const faceToCellStencil&);
    //- Disallow default bitwise copy construct
    extendedCentredFaceToCellStencil
    (
      const extendedCentredFaceToCellStencil&
    ) = delete;
    //- Disallow default bitwise assignment
    extendedCentredFaceToCellStencil& operator=
    (
      const extendedCentredFaceToCellStencil&
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
    template<class T>
    void collectData
    (
      const GeometricField<T, fvsPatchField, surfaceMesh>& fld,
      List<List<T> >& stencilFld
    ) const
    {
      extendedFaceToCellStencil::collectData
      (
        map(),
        stencil(),
        fld,
        stencilFld
      );
    }
    //- Sum surface field contributions to create cell values
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > weightedSum
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
      const List<List<scalar> >& stencilWeights
    ) const
    {
      return extendedFaceToCellStencil::weightedSum
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
