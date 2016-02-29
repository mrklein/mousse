#ifndef CORE_MATRICES_LUSCALAR_MATRIX_TLUSCALAR_MATRIX_HPP_
#define CORE_MATRICES_LUSCALAR_MATRIX_TLUSCALAR_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LUscalarMatrix
// Description
//   mousse::LUscalarMatrix
// SourceFiles
//   _luscalar_matrix.cpp
#include "scalar_matrices.hpp"
#include "label_list.hpp"
#include "field_field.hpp"
#include "ldu_interface_field_ptrs_list.hpp"
namespace mousse
{
class lduMatrix;
class procLduMatrix;
class LUscalarMatrix
:
  public scalarSquareMatrix
{
  // Private data
    //- Communicator to use
    const label comm_;
    //- Processor matrix offsets
    labelList procOffsets_;
    //- The pivot indices used in the LU decomposition
    labelList pivotIndices_;
    //- Convert the given lduMatrix into this LUscalarMatrix
    void convert
    (
      const lduMatrix& ldum,
      const FieldField<Field, scalar>& interfaceCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
    //- Convert the given list of procLduMatrix into this LUscalarMatrix
    //  on the master processor
    void convert(const PtrList<procLduMatrix>& lduMatrices);
    //- Print the ratio of the mag-sum of the off-diagonal coefficients
    //  to the mag-diagonal
    void printDiagonalDominance() const;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("LUscalarMatrix");
  // Constructors
    //- Construct from scalarSquareMatrix and perform LU decomposition
    LUscalarMatrix(const scalarSquareMatrix&);
    //- Construct from lduMatrix and perform LU decomposition
    LUscalarMatrix
    (
      const lduMatrix&,
      const FieldField<Field, scalar>& interfaceCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
  // Member Functions
    //- Solve the matrix using the LU decomposition with pivoting
    //  returning the solution in the source
    template<class T>
    void solve(Field<T>& source) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_luscalar_matrix_templates.cpp"
#endif
#endif
