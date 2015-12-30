// mousse: CFD toolbox
// Copyright (C) 2013-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::solidReactionThermo& mousse::basicSolidChemistryModel::solidThermo()
{
  return solidThermo_();
}
inline const mousse::solidReactionThermo&
mousse::basicSolidChemistryModel::solidThermo() const
{
  return solidThermo_();
}
