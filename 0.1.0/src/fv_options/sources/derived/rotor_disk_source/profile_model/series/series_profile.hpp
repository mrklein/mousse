// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::seriesProfile
// Description
//   Series-up based profile data - drag and lift coefficients computed as
//   sum of cosine series
//     Cd = sum_i(CdCoeff)*cos(i*AOA)
//     Cl = sum_i(ClCoeff)*sin(i*AOA)
//   where:
//     AOA = angle of attack [deg] converted to [rad] internally
//     Cd = drag coefficent
//     Cl = lift coefficent
//   Input in two (arbitrary length) lists:
//     CdCoeffs (coeff1 coeff2 ... coeffN);
//     ClCoeffs (coeff1 coeff2 ... coeffN);
// SourceFiles
//   series_profile.cpp
#ifndef series_profile_hpp_
#define series_profile_hpp_
#include "profile_model.hpp"
#include "list.hpp"
namespace mousse
{
class seriesProfile
:
  public profileModel
{
protected:
  // Protected data
    //- List of drag coefficient values
    List<scalar> CdCoeffs_;
    //- List of lift coefficient values
    List<scalar> ClCoeffs_;
  // Protected Member Functions
    // Evaluate
      //- Drag
      scalar evaluateDrag
      (
        const scalar& xIn,
        const List<scalar>& values
      ) const;
      //- Lift
      scalar evaluateLift
      (
        const scalar& xIn,
        const List<scalar>& values
      ) const;
public:
  //- Runtime type information
  TypeName("series");
  //- Constructor
  seriesProfile(const dictionary& dict, const word& modelName);
  // Member functions
    // Evaluation
      //- Return the Cd and Cl for a given angle-of-attack
      virtual void Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const;
};
}  // namespace mousse
#endif
