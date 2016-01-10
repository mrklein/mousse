// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"
#include "sub_list.hpp"
// Constructors 
template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList()
:
  size_(0)
{}
template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList
(
  const label nRows,
  const label nData
)
:
  size_(nRows),
  offsets_(nRows+1, 0),
  m_(nData)
{}
template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList
(
  const label nRows,
  const label nData,
  const T& t
)
:
  size_(nRows),
  offsets_(nRows+1, 0),
  m_(nData, t)
{}
template<class T, class Container>
inline mousse::autoPtr<mousse::CompactListList<T, Container> >
mousse::CompactListList<T, Container>::clone() const
{
  return autoPtr<CompactListList<T, Container> >
  (
    new CompactListList<T, Container>(*this)
  );
}
// Member Functions 
template<class T, class Container>
inline const mousse::CompactListList<T, Container>&
mousse::CompactListList<T, Container>::null()
{
  return NullObjectRef<CompactListList<T, Container> >();
}
template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::size() const
{
  return size_;
}
template<class T, class Container>
inline bool mousse::CompactListList<T, Container>::empty() const
{
  return !size_;
}
template<class T, class Container>
inline const mousse::List<mousse::label>&
mousse::CompactListList<T, Container>::offsets() const
{
  return offsets_;
}
template<class T, class Container>
inline mousse::List<mousse::label>& mousse::CompactListList<T, Container>::offsets()
{
  return offsets_;
}
template<class T, class Container>
inline const mousse::List<T>& mousse::CompactListList<T, Container>::m()
const
{
  return m_;
}
template<class T, class Container>
inline mousse::List<T>& mousse::CompactListList<T, Container>::m()
{
  return m_;
}
template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::index
(
  const label i,
  const label j
) const
{
  return offsets_[i] + j;
}
template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::whichRow(const label i)
const
{
  if (i < 0 || i >= m_.size())
  {
    FATAL_ERROR_IN
    (
      "CompactListList<T, Container>::whichRow(const label) const"
    )   << "Index " << i << " outside 0.." << m_.size()
      << abort(FatalError);
  }
  return findLower(offsets_, i+1);
}
template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::whichColumn
(
  const label row,
  const label i
) const
{
  return i - index(row, 0);
}
template<class T, class Container>
inline mousse::Xfer<mousse::CompactListList<T, Container> >
mousse::CompactListList<T, Container>::xfer()
{
  return xferMove(*this);
}
template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize(const label nRows)
{
  this->setSize(nRows);
}
template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const label nRows,
  const label nData
)
{
  this->setSize(nRows, nData);
}
template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const label nRows,
  const label nData,
  const T& t
)
{
  this->setSize(nRows, nData, t);
}
template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const labelUList& rowSizes
)
{
  this->setSize(rowSizes);
}
// Member Operators 
template<class T, class Container>
inline mousse::UList<T> mousse::CompactListList<T, Container>::operator[]
(
  const label i
)
{
  label start = offsets_[i];
  return UList<T>(m_.begin() + start, offsets_[i+1] - start);
}
template<class T, class Container>
inline const mousse::UList<T>
mousse::CompactListList<T, Container>::operator[]
(
  const label i
) const
{
  label start = offsets_[i];
  return UList<T>
  (
    const_cast<T*>(m_.begin() + start),
    offsets_[i+1] - start
  );
}
template<class T, class Container>
inline T& mousse::CompactListList<T, Container>::operator()
(
  const label i,
  const label j
)
{
  return m_[index(i, j)];
}
template<class T, class Container>
inline const T& mousse::CompactListList<T, Container>::operator()
(
  const label i,
  const label j
) const
{
  return m_[index(i, j)];
}
template<class T, class Container>
inline void mousse::CompactListList<T, Container>::operator=(const T& t)
{
  m_ = t;
}
