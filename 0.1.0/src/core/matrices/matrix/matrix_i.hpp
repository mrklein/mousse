// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Form, class Type>
inline mousse::Matrix<Form, Type>::Matrix()
:
  n_(0),
  m_(0),
  v_(NULL)
{}
template<class Form, class Type>
inline mousse::autoPtr<mousse::Matrix<Form, Type> > mousse::Matrix<Form, Type>::
clone() const
{
  return autoPtr<Matrix<Form, Type> >(new Matrix<Form, Type>(*this));
}
// Member Functions 
template<class Form, class Type>
inline const mousse::Matrix<Form, Type>& mousse::Matrix<Form, Type>::null()
{
  return NullObjectRef<Matrix<Form, Type> >();
}
//- Return the number of rows
template<class Form, class Type>
inline mousse::label mousse::Matrix<Form, Type>::n() const
{
  return n_;
}
template<class Form, class Type>
inline mousse::label mousse::Matrix<Form, Type>::m() const
{
  return m_;
}
template<class Form, class Type>
inline mousse::label mousse::Matrix<Form, Type>::size() const
{
  return n_*m_;
}
template<class Form, class Type>
inline void mousse::Matrix<Form, Type>::checki(const label i) const
{
  if (!n_)
  {
    FATAL_ERROR_IN("Matrix<Form, Type>::checki(const label)")
      << "attempt to access element from zero sized row"
      << abort(FatalError);
  }
  else if (i<0 || i>=n_)
  {
    FATAL_ERROR_IN("Matrix<Form, Type>::checki(const label)")
      << "index " << i << " out of range 0 ... " << n_-1
      << abort(FatalError);
  }
}
template<class Form, class Type>
inline void mousse::Matrix<Form, Type>::checkj(const label j) const
{
  if (!m_)
  {
    FATAL_ERROR_IN("Matrix<Form, Type>::checkj(const label)")
      << "attempt to access element from zero sized column"
      << abort(FatalError);
  }
  else if (j<0 || j>=m_)
  {
    FATAL_ERROR_IN("Matrix<Form, Type>::checkj(const label)")
      << "index " << j << " out of range 0 ... " << m_-1
      << abort(FatalError);
  }
}
// Member Operators 
template<class Form, class Type>
inline Type* mousse::Matrix<Form, Type>::operator[](const label i)
{
#   ifdef FULLDEBUG
  checki(i);
#   endif
  return v_[i];
}
template<class Form, class Type>
inline const Type* mousse::Matrix<Form, Type>::operator[](const label i) const
{
#   ifdef FULLDEBUG
  checki(i);
#   endif
  return v_[i];
}
