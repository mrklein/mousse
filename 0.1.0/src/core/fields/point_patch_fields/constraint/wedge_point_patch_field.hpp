#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_WEDGE_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_WEDGE_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgePointPatchField
// Description
//   Wedge front and back plane patch field
// SourceFiles
//   wedge_point_patch_field.cpp


#include "point_patch_field.hpp"
#include "wedge_point_patch.hpp"

namespace mousse
{
template<class Type>
class wedgePointPatchField
:
  public pointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(wedgePointPatch::typeName_());

  // Constructors
    //- Construct from patch and internal field
    wedgePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    wedgePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    wedgePointPatchField
    (
      const wedgePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      {
        new wedgePointPatchField<Type>
        {
          *this
        }
      };
    }

    //- Construct as copy setting internal field reference
    wedgePointPatchField
    (
      const wedgePointPatchField<Type>&,
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
        new wedgePointPatchField<Type>
        {
          *this, iF
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

    // Evaluation functions
      //- Update the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
};

}  // namespace mousse

#ifdef NoRepository
#    include "wedge_point_patch_field.cpp"
#endif
#endif
