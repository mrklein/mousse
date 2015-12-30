// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::speciesTable&
mousse::basicMultiComponentMixture::species() const
{
  return species_;
}
inline mousse::PtrList<mousse::volScalarField>&
mousse::basicMultiComponentMixture::Y()
{
  return Y_;
}
inline const mousse::PtrList<mousse::volScalarField>&
mousse::basicMultiComponentMixture::Y() const
{
  return Y_;
}
inline mousse::volScalarField& mousse::basicMultiComponentMixture::Y(const label i)
{
  return Y_[i];
}
inline const mousse::volScalarField& mousse::basicMultiComponentMixture::Y
(
  const label i
) const
{
  return Y_[i];
}
inline mousse::volScalarField& mousse::basicMultiComponentMixture::Y
(
  const word& specieName
)
{
  return Y_[species_[specieName]];
}
inline const mousse::volScalarField& mousse::basicMultiComponentMixture::Y
(
  const word& specieName
) const
{
  return Y_[species_[specieName]];
}
inline bool mousse::basicMultiComponentMixture::contains
(
  const word& specieName
) const
{
  return species_.contains(specieName);
}
