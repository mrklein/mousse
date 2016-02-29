#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_POROSITY_MODEL_POWER_LAW_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_POROSITY_MODEL_POWER_LAW_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::powerLaw
// Description
//   Power law porosity model, given by:
//     \f[
//       S = - \rho C_0 |U|^{(C_1 - 1)} U
//     \f]
//   where
//   \vartable
//     C_0      | model linear coefficient
//     C_1      | model exponent coefficient
//   \endvartable
// SourceFiles
//   power_law.cpp
//   power_law_templates.cpp
#include "porosity_model.hpp"
namespace mousse
{
namespace porosityModels
{
class powerLaw
:
  public porosityModel
{
  // Private data
    //- C0 coefficient
    scalar C0_;
    //- C1 coefficient
    scalar C1_;
    //- Name of density field
    word rhoName_;
  // Private Member Functions
    //- Apply resistance
    template<class RhoFieldType>
    void apply
    (
      scalarField& Udiag,
      const scalarField& V,
      const RhoFieldType& rho,
      const vectorField& U
    ) const;
    //- Apply resistance
    template<class RhoFieldType>
    void apply
    (
      tensorField& AU,
      const RhoFieldType& rho,
      const vectorField& U
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("powerLaw");
  //- Constructor
    powerLaw
    (
      const word& name,
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& cellZoneName
    );
    //- Disallow default bitwise copy construct
    powerLaw(const powerLaw&) = delete;
    //- Disallow default bitwise assignment
    powerLaw& operator=(const powerLaw&) = delete;
  //- Destructor
  virtual ~powerLaw();
  // Member Functions
    //- Transform the model data wrt mesh changes
    virtual void calcTranformModelData();
    //- Calculate the porosity force
    virtual void calcForce
    (
      const volVectorField& U,
      const volScalarField& rho,
      const volScalarField& mu,
      vectorField& force
    ) const;
    //- Add resistance
    virtual void correct(fvVectorMatrix& UEqn) const;
    //- Add resistance
    virtual void correct
    (
      fvVectorMatrix& UEqn,
      const volScalarField& rho,
      const volScalarField& mu
    ) const;
    //- Add resistance
    virtual void correct
    (
      const fvVectorMatrix& UEqn,
      volTensorField& AU
    ) const;
  // I-O
    //- Write
    bool writeData(Ostream& os) const;
};
}  // namespace porosityModels
}  // namespace mousse
#ifdef NoRepository
  #include "power_law_templates.cpp"
#endif
#endif
