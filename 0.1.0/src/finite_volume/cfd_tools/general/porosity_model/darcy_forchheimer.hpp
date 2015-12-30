// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DarcyForchheimer
// Description
//   Darcy-Forchheimer law porosity model, given by:
//     \f[
//       S = - (\mu d + \frac{\rho |U|}{2} f) U
//     \f]
//   where
//   \vartable
//     d        | Darcy coefficient [1/m2]
//     f        | Forchheimer coefficient [1/m]
//   \endvartable
//   Since negative Darcy/Forchheimer parameters are invalid, they can be used
//   to specify a multiplier (of the max component).
//   The orientation of the porous region is defined with the same notation as
//   a co-ordinate system, but only a Cartesian co-ordinate system is valid.
// SourceFiles
//   darcy_forchheimer.cpp
//   darcy_forchheimer_templates.cpp
#ifndef darcy_forchheimer_hpp_
#define darcy_forchheimer_hpp_
#include "porosity_model.hpp"
#include "dimensioned_tensor.hpp"
namespace mousse
{
namespace porosityModels
{
class DarcyForchheimer
:
  public porosityModel
{
  // Private data
    //- Darcy coeffient XYZ components (user-supplied) [1/m2]
    dimensionedVector dXYZ_;
    //- Forchheimer coeffient XYZ components (user-supplied) [1/m]
    dimensionedVector fXYZ_;
    //- Darcy coefficient - converted from dXYZ [1/m2]
    List<tensorField> D_;
    //- Forchheimer coefficient - converted from fXYZ [1/m]
    List<tensorField> F_;
    //- Name of density field
    word rhoName_;
    //- Name of dynamic viscosity field
    word muName_;
    //- Name of kinematic viscosity field
    word nuName_;
  // Private Member Functions
    //- Apply
    template<class RhoFieldType>
    void apply
    (
      scalarField& Udiag,
      vectorField& Usource,
      const scalarField& V,
      const RhoFieldType& rho,
      const scalarField& mu,
      const vectorField& U
    ) const;
    //- Apply
    template<class RhoFieldType>
    void apply
    (
      tensorField& AU,
      const RhoFieldType& rho,
      const scalarField& mu,
      const vectorField& U
    ) const;
    //- Disallow default bitwise copy construct
    DarcyForchheimer(const DarcyForchheimer&);
    //- Disallow default bitwise assignment
    void operator=(const DarcyForchheimer&);
public:
  //- Runtime type information
  TypeName("DarcyForchheimer");
  //- Constructor
  DarcyForchheimer
  (
    const word& name,
    const word& modelType,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName
  );
  //- Destructor
  virtual ~DarcyForchheimer();
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
  #include "darcy_forchheimer_templates.cpp"
#endif
#endif
