#ifndef SIX_DOF_RIGID_BODY_MOTION_POINT_PATCH_FIELDS_DERIVED_UNCOUPLED_SIX_DOF_RIGID_BODY_DISPLACEMENT_UNCOUPLED_SIX_DOF_RIGID_BODY_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_POINT_PATCH_FIELDS_DERIVED_UNCOUPLED_SIX_DOF_RIGID_BODY_DISPLACEMENT_UNCOUPLED_SIX_DOF_RIGID_BODY_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
// Description
//   mousse::uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
// SourceFiles
//   uncoupled_six_do_f_rigid_body_displacement_point_patch_vector_field.cpp
#include "fixed_value_point_patch_field.hpp"
#include "six_dof_rigid_body_motion.hpp"
namespace mousse
{
class uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    //- Six dof motion object
    sixDoFRigidBodyMotion motion_;
    //- Initial positions of points on the patch
    pointField initialPoints_;
    //- Current time index (used for updating)
    label curTimeIndex_;
public:
  //- Runtime type information
  TYPE_NAME("uncoupledSixDoFRigidBodyDisplacement");
  // Constructors
    //- Construct from patch and internal field
    uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<vector> > clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
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
        const pointPatchField<vector>&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
