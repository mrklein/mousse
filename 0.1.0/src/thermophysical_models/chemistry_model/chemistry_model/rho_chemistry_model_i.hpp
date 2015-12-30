// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::rhoReactionThermo& mousse::rhoChemistryModel::thermo()
{
  return thermo_();
}
inline const mousse::rhoReactionThermo& mousse::rhoChemistryModel::thermo() const
{
  return thermo_();
}
