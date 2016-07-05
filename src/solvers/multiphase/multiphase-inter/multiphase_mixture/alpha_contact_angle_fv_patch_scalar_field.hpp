#ifndef ALPHA_CONTACT_ANGLE_FV_PATCH_SCALAR_FIELD_HPP_
#define ALPHA_CONTACT_ANGLE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::alphaContactAngleFvPatchScalarField
// Description
//   Contact-angle boundary condition for multi-phase interface-capturing
//   simulations.  Used in conjuction with multiphaseMixture.

#include "zero_gradient_fv_patch_fields.hpp"
#include "multiphase_mixture.hpp"


namespace mousse {

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
        return (matched) ? theta0_ : 180.0 - theta0_;
      }
      //- Return the dynamic contact angle velocity scale
      scalar uTheta() const
      {
        return uTheta_;
      }
      //- Return the limiting advancing contact angle
      scalar thetaA(bool matched=true) const
      {
        return (matched) ? thetaA_ : 180.0 - thetaA_;
      }
      //- Return the limiting receeding contact angle
      scalar thetaR(bool matched=true) const
      {
        return (matched) ? thetaR_ : 180.0 - thetaR_;
      }
    // IO functions
      friend Istream& operator>>(Istream&, interfaceThetaProps&);
      friend Ostream& operator<<(Ostream&, const interfaceThetaProps&);
  };
  typedef HashTable
  <
    interfaceThetaProps,
    multiphaseMixture::interfacePair,
    multiphaseMixture::interfacePair::hash
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
      {
        new alphaContactAngleFvPatchScalarField{*this}
      };
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
      {
        new alphaContactAngleFvPatchScalarField{*this, iF}
      };
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

