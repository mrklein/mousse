#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_EMPTY_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_EMPTY_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyPointPatchField
// Description
//   An empty boundary condition for pointField
// SourceFiles
//   empty_point_patch_field.cpp


#include "point_patch_field.hpp"
#include "empty_point_patch.hpp"

namespace mousse
{

template<class Type>
class emptyPointPatchField
:
  public pointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(emptyPointPatch::typeName_());

  // Constructors
    //- Construct from patch and internal field
    emptyPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    emptyPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    emptyPointPatchField
    (
      const emptyPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      {
        new emptyPointPatchField<Type>
        {
          *this
        }
      };
    }

    //- Construct as copy setting internal field reference
    emptyPointPatchField
    (
      const emptyPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      {
        new emptyPointPatchField<Type>
        {
          *this,
          iF
        }
      };
    }

  // Member functions
    //- Constraint handling
      //- Return the constraint type this pointPatchField implements
      virtual const word& constraintType() const
      {
        return type();
      }
};

}  // namespace mousse
#ifdef NoRepository
#    include "empty_point_patch_field.cpp"
#endif
#endif
