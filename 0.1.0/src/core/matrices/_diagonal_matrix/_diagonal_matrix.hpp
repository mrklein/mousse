#ifndef CORE_MATRICES_TDIAGONAL_MATRIX_TDIAGONAL_MATRIX_HPP_
#define CORE_MATRICES_TDIAGONAL_MATRIX_TDIAGONAL_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DiagonalMatrix
// Description
//   DiagonalMatrix<Type> is a 2D diagonal matrix of objects
//   of type Type, size nxn

#include "list.hpp"


namespace mousse {

template<class Form, class Type> class Matrix;


template<class Type>
class DiagonalMatrix
:
  public List<Type>
{
public:
  // Constructors
    //- Null constructor.
    DiagonalMatrix<Type>();
    //- Construct from diagonal component of a Matrix
    template<class Form>
    DiagonalMatrix<Type>(const Matrix<Form, Type>&);
    //- Construct empty from size
    DiagonalMatrix<Type>(const label size);
    //- Construct from size and a value
    DiagonalMatrix<Type>(const label, const Type&);
  // Member functions
    //- Invert the diagonal matrix and return itself
    DiagonalMatrix<Type>& invert();
};

// Global functions

//- Return the diagonal Matrix inverse
template<class Type>
DiagonalMatrix<Type> inv(const DiagonalMatrix<Type>&);

}  // namespace mousse


#include "_diagonal_matrix.ipp"

#endif
