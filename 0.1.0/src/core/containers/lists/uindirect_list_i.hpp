// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors
template<class T>
inline mousse::UIndirectList<T>::UIndirectList
(
  const UList<T>& completeList,
  const labelUList& addr
)
:
  completeList_(const_cast<UList<T>&>(completeList)),
  addressing_(addr)
{}
// Member Functions
template<class T>
inline mousse::label mousse::UIndirectList<T>::size() const
{
  return addressing_.size();
}
template<class T>
inline bool mousse::UIndirectList<T>::empty() const
{
  return addressing_.empty();
}
template<class T>
inline T& mousse::UIndirectList<T>::first()
{
  return completeList_[addressing_.first()];
}
template<class T>
inline const T& mousse::UIndirectList<T>::first() const
{
  return completeList_[addressing_.first()];
}
template<class T>
inline T& mousse::UIndirectList<T>::last()
{
  return completeList_[addressing_.last()];
}
template<class T>
inline const T& mousse::UIndirectList<T>::last() const
{
  return completeList_[addressing_.last()];
}
template<class T>
inline const mousse::UList<T>& mousse::UIndirectList<T>::completeList() const
{
  return completeList_;
}
template<class T>
inline const mousse::List<mousse::label>& mousse::UIndirectList<T>::addressing() const
{
  return addressing_;
}
// Member Operators
template<class T>
inline mousse::List<T> mousse::UIndirectList<T>::operator()() const
{
  List<T> result(size());
  FOR_ALL(*this, i)
  {
    result[i] = operator[](i);
  }
  return result;
}
template<class T>
inline T& mousse::UIndirectList<T>::operator[](const label i)
{
  return completeList_[addressing_[i]];
}
template<class T>
inline const T& mousse::UIndirectList<T>::operator[](const label i) const
{
  return completeList_[addressing_[i]];
}
template<class T>
inline void mousse::UIndirectList<T>::operator=(const UList<T>& ae)
{
  if (addressing_.size() != ae.size())
  {
    FATAL_ERROR_IN("UIndirectList<T>::operator=(const UList<T>&)")
      << "Addressing and list of addressed elements "
       "have different sizes: "
      << addressing_.size() << " " << ae.size()
      << abort(FatalError);
  }
  FOR_ALL(addressing_, i)
  {
    completeList_[addressing_[i]] = ae[i];
  }
}
template<class T>
inline void mousse::UIndirectList<T>::operator=(const UIndirectList<T>& ae)
{
  if (addressing_.size() != ae.size())
  {
    FATAL_ERROR_IN("UIndirectList<T>::operator=(const UIndirectList<T>&)")
      << "Addressing and list of addressed elements "
         "have different sizes: "
      << addressing_.size() << " " << ae.size()
      << abort(FatalError);
  }
  FOR_ALL(addressing_, i)
  {
    completeList_[addressing_[i]] = ae[i];
  }
}
template<class T>
inline void mousse::UIndirectList<T>::operator=(const T& t)
{
  FOR_ALL(addressing_, i)
  {
    completeList_[addressing_[i]] = t;
  }
}
