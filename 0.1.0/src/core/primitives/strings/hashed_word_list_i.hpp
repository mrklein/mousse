// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline bool mousse::hashedWordList::contains(const word& name) const
{
  return indices_.found(name);
}
// Member Operators 
inline void mousse::hashedWordList::operator=(const UList<word>& lst)
{
  List<word>::operator=(lst);
  rehash();
}
inline void mousse::hashedWordList::operator=(const hashedWordList& lst)
{
  operator=(static_cast<const UList<word>&>(lst));
}
inline const mousse::word& mousse::hashedWordList::operator[]
(
  const label index
) const
{
  return List<word>::operator[](index);
}
// could return -1 instead of bombing out
inline mousse::label mousse::hashedWordList::operator[](const word& name) const
{
  return indices_[name];
}
