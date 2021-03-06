#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_OMEGA_WALL_FUNCTIONS_OMEGA_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_OMEGA_WALL_FUNCTIONS_OMEGA_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::omegaWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a wall function constraint on turbulnce
//   specific dissipation, omega.  The values are computed using:
//     \f[
//       \omega = sqrt(\omega_{vis}^2 + \omega_{log}^2)
//     \f]
//   where
//   \vartable
//     \omega_{vis} | omega in viscous region
//     \omega_{log} | omega in logarithmic region
//   \endvartable
//   Model described by Eq.(15) of:
//   \verbatim
//     Menter, F., Esch, T.
//     "Elements of Industrial Heat Transfer Prediction"
//     16th Brazilian Congress of Mechanical Engineering (COBEM),
//     Nov. 2001
//   \endverbatim
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Cmu          | model coefficient       | no          | 0.09
//     kappa        | Von Karman constant     | no          | 0.41
//     E            | model coefficient       | no          | 9.8
//     beta1        | model coefficient       | no          | 0.075
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            omegaWallFunction;
//   }
//   \endverbatim

#include "fixed_value_fv_patch_field.hpp"


namespace mousse {

class turbulenceModel;


class omegaWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchField<scalar>
{
protected:
  // Protected data
    //- Tolerance used in weighted calculations
    static scalar tolerance_;
    //- Cmu coefficient
    scalar Cmu_;
    //- Von Karman constant
    scalar kappa_;
    //- E coefficient
    scalar E_;
    //- beta1 coefficient
    scalar beta1_;
    //- Y+ at the edge of the laminar sublayer
    scalar yPlusLam_;
    //- Local copy of turbulence G field
    scalarField G_;
    //- Local copy of turbulence omega field
    scalarField omega_;
    //- Initialised flag
    bool initialised_;
    //- Master patch ID
    label master_;
    //- List of averaging corner weights
    List<List<scalar> > cornerWeights_;
  // Protected Member Functions
    //- Check the type of the patch
    virtual void checkType();
    //- Write local wall function variables
    virtual void writeLocalEntries(Ostream&) const;
    //- Set the master patch - master is responsible for updating all
    //  wall function patches
    virtual void setMaster();
    //- Create the averaging weights for cells which are bounded by
    //  multiple wall function faces
    virtual void createAveragingWeights();
    //- Helper function to return non-const access to an omega patch
    virtual omegaWallFunctionFvPatchScalarField& omegaPatch
    (
      const label patchi
    );
    //- Main driver to calculate the turbulence fields
    virtual void calculateTurbulenceFields
    (
      const turbulenceModel& turbulence,
      scalarField& G0,
      scalarField& omega0
    );
    //- Calculate the omega and G
    virtual void calculate
    (
      const turbulenceModel& turbulence,
      const List<scalar>& cornerWeights,
      const fvPatch& patch,
      scalarField& G,
      scalarField& omega
    );
    //- Return non-const access to the master patch ID
    virtual label& master()
    {
      return master_;
    }
public:
  //- Runtime type information
  TYPE_NAME("omegaWallFunction");
  // Constructors
    //- Construct from patch and internal field
    omegaWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    omegaWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // omegaWallFunctionFvPatchScalarField
    //  onto a new patch
    omegaWallFunctionFvPatchScalarField
    (
      const omegaWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    omegaWallFunctionFvPatchScalarField
    (
      const omegaWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new omegaWallFunctionFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    omegaWallFunctionFvPatchScalarField
    (
      const omegaWallFunctionFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new omegaWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Access
      //- Return non-const access to the master's G field
      scalarField& G(bool init = false);
      //- Return non-const access to the master's omega field
      scalarField& omega(bool init = false);
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs(const scalarField& weights);
      //- Manipulate matrix
      virtual void manipulateMatrix(fvMatrix<scalar>& matrix);
      //- Manipulate matrix with given weights
      virtual void manipulateMatrix
      (
        fvMatrix<scalar>& matrix,
        const scalarField& weights
      );
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

