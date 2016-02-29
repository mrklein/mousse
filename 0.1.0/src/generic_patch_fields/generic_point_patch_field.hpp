#ifndef GENERIC_PATCH_FIELDS_GENERIC_POINT_PATCH_FIELD_HPP_
#define GENERIC_PATCH_FIELDS_GENERIC_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericPointPatchField
// Description
//   A generic version of calculatedPointPatchField, useful as a fallback for
//   handling unknown patch types.
// SourceFiles
//   generic_point_patch_field.cpp
#include "calculated_point_patch_field.hpp"
#include "hash_ptr_table.hpp"
namespace mousse
{
template<class Type>
class genericPointPatchField
:
  public calculatedPointPatchField<Type>
{
  // Private data
    word actualTypeName_;
    dictionary dict_;
    HashPtrTable<scalarField> scalarFields_;
    HashPtrTable<vectorField> vectorFields_;
    HashPtrTable<sphericalTensorField> sphericalTensorFields_;
    HashPtrTable<symmTensorField> symmTensorFields_;
    HashPtrTable<tensorField> tensorFields_;
public:
  //- Runtime type information
  TYPE_NAME("generic");
  // Constructors
    //- Construct from patch and internal field
    genericPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    genericPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    genericPointPatchField
    (
      const genericPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new genericPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    genericPointPatchField
    (
      const genericPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new genericPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const pointPatchFieldMapper&
      );
      //- Reverse map the given pointPatchField onto this pointPatchField
      virtual void rmap
      (
        const pointPatchField<Type>&,
        const labelList&
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#    include "generic_point_patch_field.cpp"
#endif
#endif
