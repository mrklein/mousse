// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "series_profile.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ifstream.hpp"
namespace mousse
{
  defineTypeNameAndDebug(seriesProfile, 0);
  addToRunTimeSelectionTable(profileModel, seriesProfile, dictionary);
}
// Protected Member Functions 
mousse::scalar mousse::seriesProfile::evaluateDrag
(
  const scalar& xIn,
  const List<scalar>& values
) const
{
  scalar result = 0.0;
  forAll(values, i)
  {
    result += values[i]*cos(i*xIn);
  }
  return result;
}
mousse::scalar mousse::seriesProfile::evaluateLift
(
  const scalar& xIn,
  const List<scalar>& values
) const
{
  scalar result = 0.0;
  forAll(values, i)
  {
    // note: first contribution always zero since sin(0) = 0, but
    // keep zero base to be consitent with drag coeffs
    result += values[i]*sin(i*xIn);
  }
  return result;
}
// Constructors 
mousse::seriesProfile::seriesProfile
(
  const dictionary& dict,
  const word& modelName
)
:
  profileModel(dict, modelName),
  CdCoeffs_(),
  ClCoeffs_()
{
  if (readFromFile())
  {
    IFstream is(fName_);
    is  >> CdCoeffs_ >> ClCoeffs_;
  }
  else
  {
    dict.lookup("CdCoeffs") >> CdCoeffs_;
    dict.lookup("ClCoeffs") >> ClCoeffs_;
  }
  if (CdCoeffs_.empty())
  {
    FatalErrorIn
    (
      "mousse::seriesProfile::seriesProfile"
      "("
        "const dictionary&, "
        "const word&"
      ")"
    )   << "CdCoeffs must be specified" << exit(FatalError);
  }
  if (ClCoeffs_.empty())
  {
    FatalErrorIn
    (
      "mousse::seriesProfile::seriesProfile"
      "("
        "const dictionary&, "
        "const word&"
      ")"
    )   << "ClCoeffs must be specified" << exit(FatalError);
  }
}
// Member Functions 
void mousse::seriesProfile::Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const
{
  Cd = evaluateDrag(alpha, CdCoeffs_);
  Cl = evaluateLift(alpha, ClCoeffs_);
}
