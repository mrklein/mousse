// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
inline const Type& mousse::Distribution<Type>::binWidth() const
{
  return binWidth_;
}
template<class Type>
inline const
mousse::List<mousse::label>& mousse::Distribution<Type>::listStarts() const
{
  return listStarts_;
}
