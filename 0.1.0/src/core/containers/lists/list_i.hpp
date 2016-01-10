// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class T>
inline mousse::List<T>::List()
{}
template<class T>
inline mousse::autoPtr<mousse::List<T> > mousse::List<T>::clone() const
{
  return autoPtr<List<T> >(new List<T>(*this));
}
// Member Functions 
template<class T>
inline const mousse::List<T>& mousse::List<T>::null()
{
  return NullObjectRef<List<T> >();
}
template<class T>
inline void mousse::List<T>::resize(const label newSize)
{
  this->setSize(newSize);
}
template<class T>
inline void mousse::List<T>::resize(const label newSize, const T& a)
{
  this->setSize(newSize, a);
}
template<class T>
inline T& mousse::List<T>::newElmt(const label i)
{
  if (i >= this->size())
  {
    setSize(2*this->size());
  }
  return UList<T>::operator[](i);
}
template<class T>
inline void mousse::List<T>::size(const label n)
{
  UList<T>::size_ = n;
}
template<class T>
inline mousse::label mousse::List<T>::size() const
{
  return UList<T>::size_;
}
template<class T>
inline mousse::Xfer<mousse::List<T> > mousse::List<T>::xfer()
{
  return xferMove(*this);
}
template<class T>
inline void mousse::List<T>::append(const T& t)
{
  setSize(size()+1, t);
}
template<class T>
inline void mousse::List<T>::append(const UList<T>& lst)
{
  if (this == &lst)
  {
    FATAL_ERROR_IN
    (
      "List<T>::append(const UList<T>&)"
    )   << "attempted appending to self" << abort(FatalError);
  }
  label nextFree = this->size();
  setSize(nextFree + lst.size());
  FOR_ALL(lst, elemI)
  {
    this->operator[](nextFree++) = lst[elemI];
  }
}
template<class T>
inline void mousse::List<T>::append(const UIndirectList<T>& lst)
{
  label nextFree = this->size();
  setSize(nextFree + lst.size());
  FOR_ALL(lst, elemI)
  {
    this->operator[](nextFree++) = lst[elemI];
  }
}
// Member Operators 
template<class T>
inline void mousse::List<T>::operator=(const T& t)
{
  UList<T>::operator=(t);
}
