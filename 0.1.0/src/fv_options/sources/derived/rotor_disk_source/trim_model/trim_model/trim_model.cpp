// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "trim_model.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(trimModel, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(trimModel, dictionary);
}
// Constructors 
mousse::trimModel::trimModel
(
  const fv::rotorDiskSource& rotor,
  const dictionary& dict,
  const word& name
)
:
  rotor_(rotor),
  name_(name),
  coeffs_(dictionary::null)
{
  read(dict);
}
// Destructor 
mousse::trimModel::~trimModel()
{}
// Member Functions 
void mousse::trimModel::read(const dictionary& dict)
{
  coeffs_ = dict.subDict(name_ + "Coeffs");
}
