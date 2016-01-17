// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicLduInterfaceField
// Description
//   Abstract base class for cyclic coupled interfaces.
// SourceFiles
//   cyclic_ldu_interface_field.cpp
#ifndef cyclic_ldu_interface_field_hpp_
#define cyclic_ldu_interface_field_hpp_
#include "primitive_fields_fwd.hpp"
#include "type_info.hpp"
namespace mousse
{
class cyclicLduInterfaceField
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicLduInterfaceField");
  // Constructors
    //- Construct given coupled patch
    cyclicLduInterfaceField()
    {}
  //- Destructor
  virtual ~cyclicLduInterfaceField();
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
void mousse::cyclicLduInterfaceField::transformCoupleField
(
  Field<Type>& f
) const
{
  if (doTransform())
  {
    if (forwardT().size() == 1)
    {
      transform(f, forwardT()[0], f);
    }
    else
    {
      transform(f, forwardT(), f);
    }
  }
}
#endif
