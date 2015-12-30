// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyDisplacementPointPatchVectorField
// Description
//   mousse::sixDoFRigidBodyDisplacementPointPatchVectorField
// SourceFiles
//   six_dof_rigid_body_displacement_point_patch_vector_field.cpp
#ifndef six_dof_rigid_body_displacement_point_patch_vector_field_hpp_
#define six_dof_rigid_body_displacement_point_patch_vector_field_hpp_
#include "fixed_value_point_patch_field.hpp"
#include "six_dof_rigid_body_motion.hpp"
namespace mousse
{
class sixDoFRigidBodyDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    //- Six dof motion object
    sixDoFRigidBodyMotion motion_;
    //- Initial positions of points on the patch
    pointField initialPoints_;
    //- Reference density required by the forces object for
    //  incompressible calculations, required if rhoName == rhoInf
    scalar rhoInf_;
    //- Name of density field, optional unless used for an
    //  incompressible simulation, when this needs to be specified
    //  as rhoInf
    word rhoName_;
    //- State of gravity lookup:
    //  -1 = not determined yet, as the BC may be instantiated before g has
    //       been read into the db yet.  Determination deferred until first
    //       call to updateCoeffs.  A g keyword was not supplied to the
    //       dictionary.
    //  -2 = as for -1, but a gravity value was specified in the dictionary,
    //       specifying a value in the dictionary is considered a fatal
    //       error if g is available from the db.
    //   0 = Use this boundary condition's own value of gravity, as not
    //       available from the db.
    //   1 = Lookup gravity from db.
    label lookupGravity_;
    //- Gravity vector to store when not available from the db
    vector g_;
    //- Current time index (used for updating)
    label curTimeIndex_;
public:
  //- Runtime type information
  TypeName("sixDoFRigidBodyDisplacement");
  // Constructors
    //- Construct from patch and internal field
    sixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    sixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    sixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const sixDoFRigidBodyDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new sixDoFRigidBodyDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    sixDoFRigidBodyDisplacementPointPatchVectorField
    (
      const sixDoFRigidBodyDisplacementPointPatchVectorField&,
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
        new sixDoFRigidBodyDisplacementPointPatchVectorField
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
