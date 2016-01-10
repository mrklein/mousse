// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedCoeff
// Description
//   Fixed coefficient form of porosity model
//     \f[
//       S = - \rho_ref (\alpha + \beta |U|) U
//     \f]
//   In the case of compressible flow, a value for the reference density is
//   required
// SourceFiles
//   fixed_coeff.cpp
#ifndef fixed_coeff_hpp_
#define fixed_coeff_hpp_
#include "porosity_model.hpp"
#include "dimensioned_tensor.hpp"
namespace mousse
{
namespace porosityModels
{
class fixedCoeff
:
  public porosityModel
{
  // Private data
    //- Alpha coefficient XYZ components (user-supplied) [1/s]
    dimensionedVector alphaXYZ_;
    //- Beta coefficient XYZ components (user-supplied) [1/m]
    dimensionedVector betaXYZ_;
    //- Model alpha coefficient - converted from alphaXYZ [1/s]
    List<tensorField> alpha_;
    //- Model beta coefficient - converted from betaXYZ [1/m]
    List<tensorField> beta_;
  // Private Member Functions
    //- Apply
    void apply
    (
      scalarField& Udiag,
      vectorField& Usource,
      const scalarField& V,
      const vectorField& U,
      const scalar rho
    ) const;
    //- Apply
    void apply
    (
      tensorField& AU,
      const vectorField& U,
      const scalar rho
    ) const;
    //- Disallow default bitwise copy construct
    fixedCoeff(const fixedCoeff&);
    //- Disallow default bitwise assignment
    void operator=(const fixedCoeff&);
public:
  //- Runtime type information
  TYPE_NAME("fixedCoeff");
  //- Constructor
  fixedCoeff
  (
    const word& name,
    const word& modelType,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName
  );
  //- Destructor
  virtual ~fixedCoeff();
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
#endif
