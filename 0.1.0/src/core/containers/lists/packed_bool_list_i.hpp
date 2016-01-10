// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::PackedBoolList::PackedBoolList()
:
  PackedList<1>()
{}
inline mousse::PackedBoolList::PackedBoolList(const label size)
:
  PackedList<1>(size)
{}
inline mousse::PackedBoolList::PackedBoolList
(
  const label size,
  const bool val
)
:
  PackedList<1>(size, (val ? 1u : 0u))
{}
inline mousse::PackedBoolList::PackedBoolList(const PackedBoolList& lst)
:
  PackedList<1>(lst)
{}
inline mousse::PackedBoolList::PackedBoolList(const PackedList<1>& lst)
:
  PackedList<1>(lst)
{}
inline mousse::PackedBoolList::PackedBoolList(const Xfer<PackedBoolList>& lst)
:
  PackedList<1>()
{
  transfer(lst());
}
inline mousse::PackedBoolList::PackedBoolList(const Xfer<PackedList<1> >& lst)
:
  PackedList<1>(lst)
{}
inline mousse::PackedBoolList::PackedBoolList(const mousse::UList<bool>& lst)
:
  PackedList<1>()
{
  operator=(lst);
}
inline mousse::PackedBoolList::PackedBoolList(const labelUList& indices)
:
  PackedList<1>(indices.size(), 0u)
{
  set(indices);
}
inline mousse::PackedBoolList::PackedBoolList(const UIndirectList<label>& indices)
:
  PackedList<1>(indices.size(), 0u)
{
  set(indices);
}
inline mousse::autoPtr<mousse::PackedBoolList>
mousse::PackedBoolList::clone() const
{
  return autoPtr<PackedBoolList>(new PackedBoolList(*this));
}
// Member Functions 
inline void mousse::PackedBoolList::transfer(PackedBoolList& lst)
{
  PackedList<1>::transfer(static_cast<PackedList<1>&>(lst));
}
inline void mousse::PackedBoolList::transfer(PackedList<1>& lst)
{
  PackedList<1>::transfer(lst);
}
inline mousse::Xfer<mousse::PackedBoolList> mousse::PackedBoolList::xfer()
{
  return xferMove(*this);
}
// Member Operators 
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const bool val)
{
  PackedList<1>::operator=(val);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const PackedBoolList& lst)
{
  PackedList<1>::operator=(lst);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const PackedList<1>& lst)
{
  PackedList<1>::operator=(lst);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const labelUList& indices)
{
  clear();
  set(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const UIndirectList<label>& indices)
{
  clear();
  set(indices);
  return *this;
}
inline mousse::PackedBoolList
mousse::PackedBoolList::operator~() const
{
  PackedBoolList result(*this);
  result.flip();
  return result;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator&=(const PackedList<1>& lst)
{
  subset(lst);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator&=(const labelUList& indices)
{
  subset(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator&=(const UIndirectList<label>& indices)
{
  subset(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator|=(const PackedList<1>& lst)
{
  set(lst);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator|=(const labelUList& indices)
{
  set(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator|=(const UIndirectList<label>& indices)
{
  set(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator+=(const PackedList<1>& lst)
{
  return operator|=(lst);
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator+=(const labelUList& indices)
{
  return operator|=(indices);
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator+=(const UIndirectList<label>& indices)
{
  return operator|=(indices);
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator-=(const PackedList<1>& lst)
{
  unset(lst);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator-=(const labelUList& indices)
{
  unset(indices);
  return *this;
}
inline mousse::PackedBoolList&
mousse::PackedBoolList::operator-=(const UIndirectList<label>& indices)
{
  unset(indices);
  return *this;
}
