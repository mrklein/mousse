#ifndef CORE_MATRICES_LUSCALAR_MATRIX_PROC_LDU_MATRIX_HPP_
#define CORE_MATRICES_LUSCALAR_MATRIX_PROC_LDU_MATRIX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::procLduMatrix
// Description
//   I/O for lduMatrix and interface values.
// SourceFiles
//   proc_ldu_matrix.cpp
#include "label_list.hpp"
#include "scalar_field.hpp"
#include "field_field.hpp"
#include "ldu_interface_field_ptrs_list.hpp"
namespace mousse
{
class procLduInterface;
class lduMatrix;
// Forward declaration of friend functions and operators
class procLduMatrix;
Ostream& operator<<(Ostream&, const procLduMatrix&);
class procLduMatrix
{
  // Private data
    labelList upperAddr_;
    labelList lowerAddr_;
    scalarField diag_;
    scalarField upper_;
    scalarField lower_;
    PtrList<procLduInterface> interfaces_;
  // Private Member Functions
    //- Dissallow construction as copy
    procLduMatrix(const procLduMatrix&);
public:
  friend class LUscalarMatrix;
  // Constructors
    procLduMatrix
    (
      const lduMatrix& ldum,
      const FieldField<Field, scalar>& interfaceCoeffs,
      const lduInterfaceFieldPtrsList& interfaces
    );
    procLduMatrix(Istream& is);
  // Member functions
    label size() const
    {
      return diag_.size();
    }
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const procLduMatrix&);
};
}  // namespace mousse
#endif
