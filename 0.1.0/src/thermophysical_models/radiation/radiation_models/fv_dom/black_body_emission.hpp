// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::blackBodyEmission
// Description
//   Class black body emission
//   Table of black body emissive power taken from:
//     Modest, "Radiative Heat Transfer", pp.775-777, 1993
// SourceFiles
//   black_body_emission.cpp
#ifndef blackModyEmission_H
#define blackModyEmission_H
#include "vol_fields.hpp"
#include "dimensioned_scalar.hpp"
#include "interpolation_table.hpp"
#include "vector_2d.hpp"
namespace mousse
{
namespace radiation
{
class blackBodyEmission
{
public:
  //- Static table of black body emissive power
  static const List<Tuple2<scalar, scalar> > emissivePowerTable;
private:
  // Private data
    //- Interpolation table of black body emissive power
    mutable interpolationTable<scalar> table_;
    //- Constant C1
    const dimensionedScalar C1_;
    //- Constant C2
    const dimensionedScalar C2_;
    // Ptr List for black body emission energy field for each wavelength
    PtrList<volScalarField> bLambda_;
    // Reference to the temperature field
    const volScalarField& T_;
  // Private Member Functions
    scalar fLambdaT(const scalar lambdaT) const;
public:
  // Constructors
    //- Construct from components
    blackBodyEmission
    (
      const label nLambda,
      const volScalarField& T
    );
  //- Destructor
  ~blackBodyEmission();
  // Member functions
    // Access
      //- Black body spectrum
      inline const volScalarField& bLambda(const label lambdaI) const
      {
        return bLambda_[lambdaI];
      }
      //- Spectral emission for the black body at T and lambda
      inline dimensionedScalar EblambdaT
      (
        const dimensionedScalar& T,
        const scalar lambda
      ) const
      {
        return (C1_/(pow5(lambda)*(exp(C2_/(lambda*T)) - 1.0)));
      }
      //- Integral energy at T from lambda1 to lambda2
      tmp<mousse::volScalarField> EbDeltaLambdaT
      (
        const volScalarField& T,
        const Vector2D<scalar>& band
      ) const;
  // Edit
    // Update black body emission
    void correct(const label lambdaI, const Vector2D<scalar>& band);
};
}  // namespace mousse
}  // namespace radiation
#endif
