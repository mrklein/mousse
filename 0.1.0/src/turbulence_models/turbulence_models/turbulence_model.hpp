#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_TURBULENCE_MODEL_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_TURBULENCE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulenceModel
// Group
//   grpTurbulence
// Description
//   Abstract base class for turbulence models (RAS, LES and laminar).
// SourceFiles
//   turbulence_model.cpp
#include "iodictionary.hpp"
#include "primitive_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "fv_matrices_fwd.hpp"
#include "near_wall_dist.hpp"
#include "geometric_one_field.hpp"
namespace mousse
{
// Forward declarations
class fvMesh;
class turbulenceModel
:
  public IOdictionary
{
protected:
  // Protected data
    const Time& runTime_;
    const fvMesh& mesh_;
    const volVectorField& U_;
    const surfaceScalarField& alphaRhoPhi_;
    const surfaceScalarField& phi_;
    //- Near wall distance boundary field
    nearWallDist y_;
public:
  //- Runtime type information
  TYPE_NAME("turbulenceModel");
  //- Default name of the turbulence properties dictionary
  static const word propertiesName;
  // Constructors
    //- Construct from components
    turbulenceModel
    (
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const word& propertiesName
    );
    //- Disallow default bitwise copy construct
    turbulenceModel(const turbulenceModel&) = delete;
    //- Disallow default bitwise assignment
    turbulenceModel& operator=(const turbulenceModel&) = delete;
  //- Destructor
  virtual ~turbulenceModel()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read() = 0;
    const Time& time() const
    {
      return runTime_;
    }
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Const access to the coefficients dictionary
    virtual const dictionary& coeffDict() const = 0;
    //- Helper function to return the name of the turbulence G field
    inline word GName() const
    {
      return word(type() + ":G");
    }
    //- Access function to velocity field
    inline const volVectorField& U() const
    {
      return U_;
    }
    //- Access function to phase flux field
    inline const surfaceScalarField& alphaRhoPhi() const
    {
      return alphaRhoPhi_;
    }
    //- Return the volumetric flux field
    virtual tmp<surfaceScalarField> phi() const;
    //- Return the near wall distances
    const nearWallDist& y() const
    {
      return y_;
    }
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const = 0;
    //- Return the laminar viscosity on patch
    virtual tmp<scalarField> nu(const label patchi) const = 0;
    //- Return the turbulence viscosity
    virtual tmp<volScalarField> nut() const = 0;
    //- Return the turbulence viscosity on patch
    virtual tmp<scalarField> nut(const label patchi) const = 0;
    //- Return the effective viscosity
    virtual tmp<volScalarField> nuEff() const = 0;
    //- Return the effective viscosity on patch
    virtual tmp<scalarField> nuEff(const label patchi) const = 0;
    //- Return the laminar dynamic viscosity
    virtual tmp<volScalarField> mu() const = 0;
    //- Return the laminar dynamic viscosity on patch
    virtual tmp<scalarField> mu(const label patchi) const = 0;
    //- Return the turbulence dynamic viscosity
    virtual tmp<volScalarField> mut() const = 0;
    //- Return the turbulence dynamic viscosity on patch
    virtual tmp<scalarField> mut(const label patchi) const = 0;
    //- Return the effective dynamic viscosity
    virtual tmp<volScalarField> muEff() const = 0;
    //- Return the effective dynamic viscosity on patch
    virtual tmp<scalarField> muEff(const label patchi) const = 0;
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const = 0;
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const = 0;
    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const = 0;
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct() = 0;
};
}  // namespace mousse
#endif
