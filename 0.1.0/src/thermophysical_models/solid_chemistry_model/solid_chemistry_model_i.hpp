// mousse: CFD toolbox
// Copyright (C) 2013-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
// Member Functions 
template<class CompType, class SolidThermo>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::solidChemistryModel<CompType, SolidThermo>::RRs()
{
  return RRs_;
}
template<class CompType, class SolidThermo>
inline const mousse::PtrList<mousse::Reaction<SolidThermo> >&
mousse::solidChemistryModel<CompType, SolidThermo>::reactions() const
{
  return reactions_;
}
template<class CompType, class SolidThermo>
inline mousse::label
mousse::solidChemistryModel<CompType, SolidThermo>::
nReaction() const
{
  return nReaction_;
}
template<class CompType, class SolidThermo>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::solidChemistryModel<CompType, SolidThermo>::RRs
(
  const label i
) const
{
  return RRs_[i];
}
template<class CompType, class SolidThermo>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::solidChemistryModel<CompType, SolidThermo>::RRs() const
{
  tmp<DimensionedField<scalar, volMesh> > tRRs
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "RRs",
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
    DimensionedField<scalar, volMesh>& RRs = tRRs();
    for (label i=0; i < nSolids_; i++)
    {
      RRs += RRs_[i];
    }
  }
  return tRRs;
}
