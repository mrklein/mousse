// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPointPatchField
// Description
//   A Symmetry boundary condition for pointField
// SourceFiles
//   symmetry_point_patch_field.cpp

#ifndef symmetry_point_patch_field_hpp_
#define symmetry_point_patch_field_hpp_

#include "basic_symmetry_point_patch_field.hpp"
#include "symmetry_point_patch.hpp"

namespace mousse
{

template<class Type>
class symmetryPointPatchField
:
  public basicSymmetryPointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(symmetryPointPatch::typeName_());

  // Constructors
    //- Construct from patch and internal field
    symmetryPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    symmetryPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    symmetryPointPatchField
    (
      const symmetryPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      {
        new symmetryPointPatchField<Type>
        {
          *this
        }
      };
    }

    //- Construct as copy setting internal field reference
    symmetryPointPatchField
    (
      const symmetryPointPatchField<Type>&,
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
        new symmetryPointPatchField<Type>
        {
          *this,
          iF
        }
      };
    }

  // Member functions
    //- Return the constraint type this pointPatchField implements
    virtual const word& constraintType() const
    {
      return symmetryPointPatch::typeName;
    }
};

}  // namespace mousse

#ifdef NoRepository
#    include "symmetry_point_patch_field.cpp"
#endif
#endif
