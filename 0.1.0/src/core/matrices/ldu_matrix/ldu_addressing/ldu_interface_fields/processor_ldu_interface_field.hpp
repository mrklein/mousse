#ifndef CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_FIELDS_PROCESSOR_LDU_INTERFACE_FIELD_HPP_
#define CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_FIELDS_PROCESSOR_LDU_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorLduInterfaceField
// Description
//   Abstract base class for processor coupled interfaces.

#include "primitive_fields_fwd.hpp"
#include "type_info.hpp"


namespace mousse {

class processorLduInterfaceField
{
public:
  //- Runtime type information
  TYPE_NAME("processorLduInterfaceField");
  // Constructors
    //- Construct given coupled patch
    processorLduInterfaceField()
    {}
  //- Destructor
  virtual ~processorLduInterfaceField();
  // Member Functions
    // Access
      //- Return communicator used for comms
      virtual label comm() const = 0;
      //- Return processor number
      virtual int myProcNo() const = 0;
      //- Return neigbour processor number
      virtual int neighbProcNo() const = 0;
      //- Is the transform required
      virtual bool doTransform() const = 0;
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const = 0;
      //- Return rank of component for transform
      virtual int rank() const = 0;
    //- Transform given patch field
    template<class Type>
    void transformCoupleField(Field<Type>& f) const;
    //- Transform given patch component field
    void transformCoupleField
    (
      scalarField& f,
      const direction cmpt
    ) const;
};

}  // namespace mousse


#include "tensor_field.hpp"


template<class Type>
void mousse::processorLduInterfaceField::transformCoupleField
(
  Field<Type>& f
) const
{
  if (doTransform()) {
    if (forwardT().size() == 1) {
      transform(f, forwardT()[0], f);
    } else {
      transform(f, forwardT(), f);
    }
  }
}

#endif
