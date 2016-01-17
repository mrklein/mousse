// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCentredCellToFaceStencil
// Description
// SourceFiles
//   extended_centred_cell_to_face_stencil.cpp
#ifndef extended_centred_cell_to_face_stencil_hpp_
#define extended_centred_cell_to_face_stencil_hpp_
#include "extended_cell_to_face_stencil.hpp"
namespace mousse
{
class cellToFaceStencil;
class extendedCentredCellToFaceStencil
:
  public extendedCellToFaceStencil
{
  // Private data
    //- Swap map for getting neigbouring data
    autoPtr<mapDistribute> mapPtr_;
    //- Per face the stencil.
    labelListList stencil_;
public:
  // Constructors
    //- Construct from uncompacted face stencil
    explicit extendedCentredCellToFaceStencil(const cellToFaceStencil&);
    //- Disallow default bitwise copy construct
    extendedCentredCellToFaceStencil
    (
      const extendedCentredCellToFaceStencil&
    ) = delete;
    //- Disallow default bitwise assignment
    extendedCentredCellToFaceStencil& operator=
    (
      const extendedCentredCellToFaceStencil&
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
      const GeometricField<T, fvPatchField, volMesh>& fld,
      List<List<T> >& stencilFld
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
    //- Sum vol field contributions to create face values
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > weightedSum
    (
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      const List<List<scalar> >& stencilWeights
    ) const
    {
      return extendedCellToFaceStencil::weightedSum
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
