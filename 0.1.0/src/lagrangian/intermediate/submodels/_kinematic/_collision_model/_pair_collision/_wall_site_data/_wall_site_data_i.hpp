// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#// Member Functions 
template<class Type>
mousse::label mousse::WallSiteData<Type>::patchIndex() const
{
  return patchI_;
}
template<class Type>
mousse::label& mousse::WallSiteData<Type>::patchIndex()
{
  return patchI_;
}
template<class Type>
const Type& mousse::WallSiteData<Type>::wallData() const
{
  return wallData_;
}
template<class Type>
Type& mousse::WallSiteData<Type>::wallData()
{
  return wallData_;
}
