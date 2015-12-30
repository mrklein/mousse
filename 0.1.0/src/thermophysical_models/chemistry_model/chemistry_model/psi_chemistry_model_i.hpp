// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::psiReactionThermo& mousse::psiChemistryModel::thermo()
{
  return thermo_();
}
inline const mousse::psiReactionThermo& mousse::psiChemistryModel::thermo() const
{
  return thermo_();
}
