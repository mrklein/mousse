// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::alphaContactAngleFvPatchScalarField
// Description
//   Contact-angle boundary condition for multi-phase interface-capturing
//   simulations.  Used in conjuction with multiphaseSystem.
// SourceFiles
//   alpha_contact_angle_fv_patch_scalar_field.cpp
#ifndef alpha_contact_angle_fv_patch_scalar_field_hpp_
#define alpha_contact_angle_fv_patch_scalar_field_hpp_
#include "zero_gradient_fv_patch_fields.hpp"
#include "multiphase_system.hpp"
namespace mousse
{
class alphaContactAngleFvPatchScalarField
:
  public zeroGradientFvPatchScalarField
{
public:
  class interfaceThetaProps
  {
    //- Equilibrium contact angle
    scalar theta0_;
    //- Dynamic contact angle velocity scale
    scalar uTheta_;
    //- Limiting advancing contact angle
    scalar thetaA_;
    //- Limiting receeding contact angle
    scalar thetaR_;
  public:
    // Constructors
      interfaceThetaProps()
      {}
      interfaceThetaProps(Istream&);
    // Member functions
      //- Return the equilibrium contact angle theta0
      scalar theta0(bool matched=true) const
      {
        if (matched) return theta0_;
        else return 180.0 - theta0_;
      }
      //- Return the dynamic contact angle velocity scale
      scalar uTheta() const
      {
        return uTheta_;
      }
      //- Return the limiting advancing contact angle
      scalar thetaA(bool matched=true) const
      {
        if (matched) return thetaA_;
        else return 180.0 - thetaA_;
      }
      //- Return the limiting receeding contact angle
      scalar thetaR(bool matched=true) const
      {
        if (matched) return thetaR_;
        else return 180.0 - thetaR_;
      }
    // IO functions
      friend Istream& operator>>(Istream&, interfaceThetaProps&);
      friend Ostream& operator<<(Ostream&, const interfaceThetaProps&);
  };
  typedef HashTable
  <
    interfaceThetaProps,
    phasePairKey,
    phasePairKey::hash
  > thetaPropsTable;
private:
  // Private data
    thetaPropsTable thetaProps_;
public:
  //- Runtime type information
  TYPE_NAME("alphaContactAngle");
  // Constructors
    //- Construct from patch and internal field
    alphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given alphaContactAngleFvPatchScalarField
    //  onto a new patch
    alphaContactAngleFvPatchScalarField
    (
      const alphaContactAngleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new alphaContactAngleFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    alphaContactAngleFvPatchScalarField
    (
      const alphaContactAngleFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      (
        new alphaContactAngleFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    //- Return the contact angle properties
    const thetaPropsTable& thetaProps() const
    {
      return thetaProps_;
    }
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
