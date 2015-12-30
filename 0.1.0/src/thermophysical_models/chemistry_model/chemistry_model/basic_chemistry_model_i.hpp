// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::fvMesh& mousse::basicChemistryModel::mesh() const
{
  return mesh_;
}
inline mousse::Switch mousse::basicChemistryModel::chemistry() const
{
  return chemistry_;
}
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicChemistryModel::deltaTChem() const
{
  return deltaTChem_;
}
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicChemistryModel::deltaTChem()
{
  return deltaTChem_;
}
