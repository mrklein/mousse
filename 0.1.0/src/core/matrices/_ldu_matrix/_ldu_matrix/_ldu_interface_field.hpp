#ifndef CORE_MATRICES_TLDU_MATRIX_TLDU_MATRIX_TLDU_INTERFACE_FIELD_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TLDU_MATRIX_TLDU_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LduInterfaceField
// Description
//   An abstract base class for implicitly-coupled interface fields
//   e.g. processor and cyclic patch fields.

#include "ldu_interface_field.hpp"
#include "primitive_fields_fwd.hpp"
#include "pstream.hpp"


namespace mousse {

class lduMatrix;


template<class Type>
class LduInterfaceField
:
  public lduInterfaceField
{
public:
  //- Runtime type information
  TYPE_NAME("LduInterfaceField");

  // Constructors

    //- Construct given coupled patch
    LduInterfaceField(const lduInterface& patch)
    :
      lduInterfaceField(patch)
    {}

    //- Disallow default bitwise copy construct
    LduInterfaceField(const LduInterfaceField&) = delete;

    //- Disallow default bitwise assignment
    LduInterfaceField& operator=(const LduInterfaceField&) = delete;

  //- Destructor
  virtual ~LduInterfaceField();

  // Member Functions

    // Coupled interface functionality

      //- Inherit initInterfaceMatrixUpdate from lduInterfaceField
      using lduInterfaceField::initInterfaceMatrixUpdate;
      //- Initialise neighbour matrix update
      virtual void initInterfaceMatrixUpdate
      (
        Field<Type>&,
        const Field<Type>&,
        const scalarField&,
        const Pstream::commsTypes
      ) const
      {}

      //- Inherit updateInterfaceMatrix from lduInterfaceField
      using lduInterfaceField::updateInterfaceMatrix;

      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<Type>&,
        const Field<Type>&,
        const scalarField&,
        const Pstream::commsTypes commsType
      ) const = 0;

};

}  // namespace mousse

#include "_ldu_interface_field.ipp"

#endif
