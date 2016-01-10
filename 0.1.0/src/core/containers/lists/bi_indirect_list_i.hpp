// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class T>
inline mousse::BiIndirectList<T>::BiIndirectList
(
  const UList<T>& posList,
  const UList<T>& negList,
  const labelUList& addr
)
:
  posList_{const_cast<UList<T>&>(posList)},
  negList_{const_cast<UList<T>&>(negList)},
  addressing_{addr}
{}


template<class T>
inline mousse::BiIndirectList<T>::BiIndirectList
(
  const UList<T>& posList,
  const UList<T>& negList,
  const Xfer<List<label> >& addr
)
:
  posList_{const_cast<UList<T>&>(posList)},
  negList_{const_cast<UList<T>&>(negList)},
  addressing_{addr}
{}


// Member Functions 
template<class T>
inline mousse::label mousse::BiIndirectList<T>::size() const
{
  return addressing_.size();
}


template<class T>
inline bool mousse::BiIndirectList<T>::empty() const
{
  return addressing_.empty();
}


template<class T>
inline const mousse::UList<T>& mousse::BiIndirectList<T>::posList() const
{
  return posList_;
}


template<class T>
inline const mousse::UList<T>& mousse::BiIndirectList<T>::negList() const
{
  return negList_;
}


template<class T>
inline const mousse::List<mousse::label>& mousse::BiIndirectList<T>::addressing()
const
{
  return addressing_;
}


template<class T>
inline void mousse::BiIndirectList<T>::resetAddressing
(
  const labelUList& addr
)
{
  addressing_ = addr;
}


template<class T>
inline void mousse::BiIndirectList<T>::resetAddressing
(
  const Xfer<List<label> >& addr
)
{
  addressing_.transfer(addr());
}


template<class T>
inline mousse::label mousse::BiIndirectList<T>::posIndex(const label i)
{
  return i;
}


template<class T>
inline mousse::label mousse::BiIndirectList<T>::negIndex(const label i)
{
  return -i - 1;
}


// Member Operators 
template<class T>
inline mousse::List<T> mousse::BiIndirectList<T>::operator()() const
{
  List<T> result(size());
  FOR_ALL(*this, i)
  {
    result[i] = operator[](i);
  }
  return result;
}


template<class T>
inline T& mousse::BiIndirectList<T>::operator[](const label i)
{
  label index = addressing_[i];
  if (index >= 0)
  {
    return posList_[index];
  }
  else
  {
    return negList_[-index-1];
  }
}


template<class T>
inline const T& mousse::BiIndirectList<T>::operator[](const label i) const
{
  label index = addressing_[i];
  if (index >= 0)
  {
    return posList_[index];
  }
  else
  {
    return negList_[-index-1];
  }
}


template<class T>
inline void mousse::BiIndirectList<T>::operator=(const UList<T>& ae)
{
  if (addressing_.size() != ae.size())
  {
    FATAL_ERROR_IN("BiIndirectList<T>::operator=(const UList<T>&)")
      << "Addressing and list of addressed elements "
       "have different sizes: "
      << addressing_.size() << " " << ae.size()
      << abort(FatalError);
  }
  FOR_ALL(addressing_, i)
  {
    operator[](i) = ae[i];
  }
}


template<class T>
inline void mousse::BiIndirectList<T>::operator=(const T& t)
{
  FOR_ALL(addressing_, i)
  {
    operator[](i) = t;
  }
}
