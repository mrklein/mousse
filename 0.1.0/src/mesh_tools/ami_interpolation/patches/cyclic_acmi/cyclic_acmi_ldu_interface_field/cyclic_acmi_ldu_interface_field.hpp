#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_LDU_INTERFACE_FIELD_CYCLIC_ACMI_LDU_INTERFACE_FIELD_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_LDU_INTERFACE_FIELD_CYCLIC_ACMI_LDU_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMILduInterfaceField
// Description
//   Abstract base class for cyclic ACMI coupled interfaces
// SourceFiles
//   cyclic_acmi_ldu_interface_field.cpp
#include "cyclic_ami_ldu_interface_field.hpp"
namespace mousse
{
class cyclicACMILduInterfaceField
:
  public cyclicAMILduInterfaceField
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicACMILduInterfaceField");
  // Constructors
    //- Construct null
    cyclicACMILduInterfaceField()
    {}
  //- Destructor
  virtual ~cyclicACMILduInterfaceField();
  // Member Functions
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
void mousse::cyclicACMILduInterfaceField::transformCoupleField
(
  Field<Type>& f
) const
{
  cyclicAMILduInterfaceField::transformCoupleField(f);
}
#endif
