#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_LDU_INTERFACE_FIELD_CYCLIC_AMI_LDU_INTERFACE_FIELD_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_LDU_INTERFACE_FIELD_CYCLIC_AMI_LDU_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMILduInterfaceField
// Description
//   Abstract base class for cyclic AMI coupled interfaces

#include "primitive_fields_fwd.hpp"
#include "type_info.hpp"


namespace mousse {

class cyclicAMILduInterfaceField
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicAMILduInterfaceField");
  // Constructors
    //- Construct null
    cyclicAMILduInterfaceField()
    {}
  //- Destructor
  virtual ~cyclicAMILduInterfaceField();
  // Member Functions
    // Access
      //- Is the transform required
      virtual bool doTransform() const = 0;
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const = 0;
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const = 0;
      //- Return rank of component for transform
      virtual int rank() const = 0;
    //- Transform given patch field
    template<class Type>
    void transformCoupleField(Field<Type>& f) const;
    //- Transform given patch internal field
    void transformCoupleField
    (
      scalarField& psiInternal,
      const direction cmpt
    ) const;
};

}  // namespace mousse


#include "tensor_field.hpp"


template<class Type>
void mousse::cyclicAMILduInterfaceField::transformCoupleField
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

