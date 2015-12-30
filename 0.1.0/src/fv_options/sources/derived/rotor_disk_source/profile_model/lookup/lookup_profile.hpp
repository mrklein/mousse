// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lookupProfile
// Description
//   Look-up based profile data - drag and lift coefficients are lineraly
//   interpolated based on the supplied angle of attack
//   Input in list format:
//     data
//     (
//       (AOA1 Cd1 Cl2)
//       (AOA2 Cd2 Cl2)
//       ...
//       (AOAN CdN CdN)
//     );
//   where:
//     AOA = angle of attack [deg] converted to [rad] internally
//     Cd  = drag coefficient
//     Cl  = lift coefficient
// SourceFiles
//   lookup_profile.cpp
#ifndef lookup_profile_hpp_
#define lookup_profile_hpp_
#include "profile_model.hpp"
#include "list.hpp"
namespace mousse
{
class lookupProfile
:
  public profileModel
{
protected:
  // Protected data
    //- List of angle-of-attack values [deg] on input, converted to [rad]
    List<scalar> AOA_;
    //- List of drag coefficient values
    List<scalar> Cd_;
    //- List of lift coefficient values
    List<scalar> Cl_;
  // Protected Member Functions
    //- Return the interpolation indices and gradient
    void interpolateWeights
    (
      const scalar& xIn,
      const List<scalar>& values,
      label& i1,
      label& i2,
      scalar& ddx
    ) const;
public:
  //- Runtime type information
  TypeName("lookup");
  //- Constructor
  lookupProfile(const dictionary& dict, const word& modelName);
  // Member functions
    // Evaluation
      //- Return the Cd and Cl for a given angle-of-attack
      virtual void Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const;
};
}  // namespace mousse
#endif
