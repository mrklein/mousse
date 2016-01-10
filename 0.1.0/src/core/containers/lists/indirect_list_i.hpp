// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::IndirectListAddressing::IndirectListAddressing
(
  const labelUList& addr
)
:
  addressing_(addr)
{}
inline mousse::IndirectListAddressing::IndirectListAddressing
(
  const Xfer<List<label> >& addr
)
:
  addressing_(addr)
{}
template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UList<T>& completeList,
  const labelUList& addr
)
:
  IndirectListAddressing(addr),
  UIndirectList<T>
  (
    completeList,
    IndirectListAddressing::addressing()
  )
{}
template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UList<T>& completeList,
  const Xfer<List<label> >& addr
)
:
  IndirectListAddressing(addr),
  UIndirectList<T>
  (
    completeList,
    IndirectListAddressing::addressing()
  )
{}
template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const IndirectList<T>& lst
)
:
  IndirectListAddressing(lst.addressing()),
  UIndirectList<T>
  (
    lst.completeList(),
    IndirectListAddressing::addressing()
  )
{}
template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UIndirectList<T>& lst
)
:
  IndirectListAddressing(lst.addressing()),
  UIndirectList<T>
  (
    lst.completeList(),
    IndirectListAddressing::addressing()
  )
{}
// Member Functions 
inline const mousse::List<mousse::label>&
mousse::IndirectListAddressing::addressing() const
{
  return addressing_;
}
inline void mousse::IndirectListAddressing::resetAddressing
(
  const labelUList& addr
)
{
  addressing_ = addr;
}
inline void mousse::IndirectListAddressing::resetAddressing
(
  const Xfer<List<label> >& addr
)
{
  addressing_.transfer(addr());
}
