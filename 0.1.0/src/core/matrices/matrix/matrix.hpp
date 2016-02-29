#ifndef CORE_MATRICES_MATRIX_MATRIX_HPP_
#define CORE_MATRICES_MATRIX_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Matrix
// Description
//   A templated 2D matrix of objects of \<T\>, where the n x m matrix
//   dimensions are known and used for subscript bounds checking, etc.
// SourceFiles
//   matrix.cpp
//   matrix_io.cpp


#include "bool.hpp"
#include "label.hpp"
#include "ulabel.hpp"
#include "list.hpp"
#include "auto_ptr.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class Form, class Type> class Matrix;
template<class Form, class Type> Istream& operator>>
(
  Istream&,
  Matrix<Form, Type>&
);
template<class Form, class Type> Ostream& operator<<
(
  Ostream&,
  const Matrix<Form, Type>&
);


template<class Form, class Type>
class Matrix
{
  // Private data
    //- Number of rows and columns in Matrix.
    label n_, m_;

    //- Row pointers
    Type** __restrict__ v_;

    //- Allocate the storage for the row-pointers and the data
    //  and set the row pointers
    void allocate();

public:
  // Static Member Functions

    //- Return a null Matrix
    inline static const Matrix<Form, Type>& null();

  // Constructors

    //- Null constructor.
    inline Matrix();

    //- Construct given number of rows and columns.
    Matrix(const label n, const label m);

    //- Construct with given number of rows and columns
    //  and value for all elements.
    Matrix(const label n, const label m, const Type&);

    //- Copy constructor.
    Matrix(const Matrix<Form, Type>&);

    //- Construct from Istream.
    Matrix(Istream&);

    //- Clone
    inline autoPtr<Matrix<Form, Type> > clone() const;

  //- Destructor
  ~Matrix();

  // Member Functions

    // Access

      //- Return the number of rows
      inline label n() const;

      //- Return the number of columns
      inline label m() const;

      //- Return the number of elements in matrix (n*m)
      inline label size() const;

    // Check
      //- Check index i is within valid range (0 ... n-1).
      inline void checki(const label i) const;

      //- Check index j is within valid range (0 ... m-1).
      inline void checkj(const label j) const;

    // Edit

      //- Clear the Matrix, i.e. set sizes to zero.
      void clear();

      //- Transfer the contents of the argument Matrix into this Matrix
      //  and annul the argument Matrix.
      void transfer(Matrix<Form, Type>&);

    //- Return the transpose of the matrix
    Form T() const;

  // Member operators

    //- Return subscript-checked row of Matrix.
    inline Type* operator[](const label);

    //- Return subscript-checked row of constant Matrix.
    inline const Type* operator[](const label) const;

    //- Assignment operator. Takes linear time.
    void operator=(const Matrix<Form, Type>&);

    //- Assignment of all entries to the given value
    void operator=(const Type&);

  // IOstream operators

    //- Read Matrix from Istream, discarding contents of existing Matrix.
    friend Istream& operator>> <Form, Type>
    (
      Istream&,
      Matrix<Form, Type>&
    );

    // Write Matrix to Ostream.
    friend Ostream& operator<< <Form, Type>
    (
      Ostream&,
      const Matrix<Form, Type>&
    );

};

// Global functions and operators
template<class Form, class Type> const Type& max(const Matrix<Form, Type>&);
template<class Form, class Type> const Type& min(const Matrix<Form, Type>&);
template<class Form, class Type> Form operator-(const Matrix<Form, Type>&);
template<class Form, class Type> Form operator+
(
  const Matrix<Form, Type>&,
  const Matrix<Form, Type>&
);
template<class Form, class Type> Form operator-
(
  const Matrix<Form, Type>&,
  const Matrix<Form, Type>&
);
template<class Form, class Type> Form operator*
(
  const scalar,
  const Matrix<Form, Type>&
);
template<class Form, class Type> Form operator*
(
  const Matrix<Form, Type>&,
  const Matrix<Form, Type>&
);
}  // namespace mousse

// Constructors 
template<class Form, class Type>
inline mousse::Matrix<Form, Type>::Matrix()
:
  n_{0},
  m_{0},
  v_{NULL}
{}


template<class Form, class Type>
inline mousse::autoPtr<mousse::Matrix<Form, Type> > mousse::Matrix<Form, Type>::
clone() const
{
  return autoPtr<Matrix<Form, Type>>{new Matrix<Form, Type>{*this}};
}


// Member Functions 
template<class Form, class Type>
inline const mousse::Matrix<Form, Type>& mousse::Matrix<Form, Type>::null()
{
  return NullObjectRef<Matrix<Form, Type>>();
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
#ifdef FULLDEBUG
  checki(i);
#endif
  return v_[i];
}
template<class Form, class Type>
inline const Type* mousse::Matrix<Form, Type>::operator[](const label i) const
{
#ifdef FULLDEBUG
  checki(i);
#endif
  return v_[i];
}

#ifdef NoRepository
#   include "matrix.cpp"
#endif
#endif
