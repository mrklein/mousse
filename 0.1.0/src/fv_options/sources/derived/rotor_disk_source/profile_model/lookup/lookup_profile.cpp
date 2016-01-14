// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lookup_profile.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vector.hpp"
#include "unit_conversion.hpp"
#include "ifstream.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(lookupProfile, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(profileModel, lookupProfile, dictionary);
}
// Protected Member Functions 
void mousse::lookupProfile::interpolateWeights
(
  const scalar& xIn,
  const List<scalar>& values,
  label& i1,
  label& i2,
  scalar& ddx
) const
{
  i2 = 0;
  label nElem = values.size();
  if (nElem == 1)
  {
    i1 = i2;
    ddx = 0.0;
    return;
  }
  else
  {
    while ((i2 < nElem) && (values[i2] < xIn))
    {
      i2++;
    }
    if (i2 == 0)
    {
      i1 = i2;
      ddx = 0.0;
      return;
    }
    else if (i2 == nElem)
    {
      i2 = nElem - 1;
      i1 = i2;
      ddx = 0.0;
      return;
    }
    else
    {
      i1 = i2 - 1;
      ddx = (xIn - values[i1])/(values[i2] - values[i1]);
    }
  }
}
// Constructors 
mousse::lookupProfile::lookupProfile
(
  const dictionary& dict,
  const word& modelName
)
:
  profileModel(dict, modelName),
  AOA_(),
  Cd_(),
  Cl_()
{
  List<vector> data;
  if (readFromFile())
  {
    IFstream is(fName_);
    is  >> data;
  }
  else
  {
    dict.lookup("data") >> data;
  }
  if (data.size() > 0)
  {
    AOA_.setSize(data.size());
    Cd_.setSize(data.size());
    Cl_.setSize(data.size());
    FOR_ALL(data, i)
    {
      AOA_[i] = degToRad(data[i][0]);
      Cd_[i] = data[i][1];
      Cl_[i] = data[i][2];
    }
  }
  else
  {
    FATAL_ERROR_IN
    (
      "mousse::lookupProfile::lookupProfile"
      "("
        "const dictionary&, "
        "const word&"
      ")"
    )   << "No profile data specified" << exit(FatalError);
  }
}
// Member Functions 
void mousse::lookupProfile::Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const
{
  label i1 = -1;
  label i2 = -1;
  scalar invAlpha = -1.0;
  interpolateWeights(alpha, AOA_, i1, i2, invAlpha);
  Cd = invAlpha*(Cd_[i2] - Cd_[i1]) + Cd_[i1];
  Cl = invAlpha*(Cl_[i2] - Cl_[i1]) + Cl_[i1];
}
