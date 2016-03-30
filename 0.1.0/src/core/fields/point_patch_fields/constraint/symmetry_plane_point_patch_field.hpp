#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_SYMMETRY_PLANE_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_SYMMETRY_PLANE_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlanePointPatchField
// Description
//   A symmetry-plane boundary condition for pointField

#include "basic_symmetry_point_patch_field.hpp"
#include "symmetry_plane_point_patch.hpp"


namespace mousse {

template<class Type>
class symmetryPlanePointPatchField
:
  public basicSymmetryPointPatchField<Type>
{
  // Private data
    //- Local reference cast into the symmetryPlane patch
    const symmetryPlanePointPatch& symmetryPlanePatch_;

public:
  //- Runtime type information
  TYPE_NAME(symmetryPlanePointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    symmetryPlanePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    symmetryPlanePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    symmetryPlanePointPatchField
    (
      const symmetryPlanePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      {
        new symmetryPlanePointPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    symmetryPlanePointPatchField
    (
      const symmetryPlanePointPatchField<Type>&,
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
        new symmetryPlanePointPatchField<Type>{*this, iF}
      };
    }

  // Member functions
    //- Return the constraint type this pointPatchField implements
    virtual const word& constraintType() const
    {
      return symmetryPlanePointPatch::typeName;
    }

    // Evaluation functions
      //- Update the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
};

}  // namespace mousse

#include "symmetry_plane_point_patch_field.ipp"

#endif
