// mousse: CFD toolbox
// Copyright (C) 2013-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
// Member Functions 
template<class CompType, class SolidThermo, class GasThermo>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::RRg()
{
  return RRg_;
}
template<class CompType, class SolidThermo, class GasThermo>
inline const mousse::PtrList<GasThermo>&
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
gasThermo() const
{
  return gasThermo_;
}
template<class CompType, class SolidThermo, class GasThermo>
inline const mousse::speciesTable&
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
gasTable() const
{
  return pyrolisisGases_;
}
template<class CompType, class SolidThermo, class GasThermo>
inline mousse::label
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
nSpecie() const
{
  return nSpecie_;
}
template<class CompType, class SolidThermo, class GasThermo>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::RRg
(
  const label i
) const
{
  return RRg_[i];
}
template<class CompType, class SolidThermo, class GasThermo>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
RRg() const
{
  tmp<DimensionedField<scalar, volMesh> > tRRg
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "RRg",
        this->time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      this->mesh(),
      dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
    )
  );
  if (this->chemistry_)
  {
    DimensionedField<scalar, volMesh>& RRg = tRRg();
    for (label i=0; i < nGases_; i++)
    {
      RRg += RRg_[i];
    }
  }
  return tRRg;
}
