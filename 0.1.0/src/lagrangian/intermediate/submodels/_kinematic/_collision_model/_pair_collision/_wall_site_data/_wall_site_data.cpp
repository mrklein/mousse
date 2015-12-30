// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_site_data.hpp"
// Constructors 
template<class Type>
mousse::WallSiteData<Type>::WallSiteData()
:
  patchI_(),
  wallData_()
{}
template<class Type>
mousse::WallSiteData<Type>::WallSiteData
(
  label patchI,
  const Type& wallData
)
:
  patchI_(patchI),
  wallData_(wallData)
{}
// Destructor 
template<class Type>
mousse::WallSiteData<Type>::~WallSiteData()
{}
// Member Operators 
template<class Type>
bool mousse::WallSiteData<Type>::operator==
(
  const WallSiteData<Type>& rhs
) const
{
  return patchI_ == rhs.patchI_ && wallData_ == rhs.wallData_;
}
template<class Type>
bool mousse::WallSiteData<Type>::operator!=
(
  const WallSiteData<Type>& rhs
) const
{
  return !(*this == rhs);
}
// IOstream Operators 
template<class Type>
mousse::Istream& mousse::operator>>
(
  Istream& is,
  WallSiteData<Type>& wIS
)
{
  is  >> wIS.patchI_ >> wIS.wallData_;
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>"
    "(mousse::Istream&, mousse::WallSiteData<Type>&)"
  );
  return is;
}
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const WallSiteData<Type>& wIS
)
{
  os  << wIS.patchI_ << token::SPACE << wIS.wallData_;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<"
    "(Ostream&, const WallSiteData<Type>&)"
  );
  return os;
}
