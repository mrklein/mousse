#ifndef CORE_MATRICES_LUSCALAR_MATRIX_PROC_LDU_INTERFACE_HPP_
#define CORE_MATRICES_LUSCALAR_MATRIX_PROC_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::procLduInterface
// Description
//   IO interface for processorLduInterface

#include "label_list.hpp"
#include "scalar_field.hpp"


namespace mousse {

class lduInterfaceField;


class procLduInterface
{
  // Private data
    labelList faceCells_;
    scalarField coeffs_;
    label myProcNo_;
    label neighbProcNo_;
    label tag_;
    label comm_;
  // Private Member Functions
public:
  friend class LUscalarMatrix;
  // Constructors
    procLduInterface
    (
      const lduInterfaceField& interface,
      const scalarField& coeffs
    );
    procLduInterface(Istream& is);
    autoPtr<procLduInterface> clone()
    {
      NOT_IMPLEMENTED("procLduInterface::clone()");
      return autoPtr<procLduInterface>{nullptr};
    }
    static autoPtr<procLduInterface> New(Istream& is)
    {
      return autoPtr<procLduInterface>{new procLduInterface{is}};
    }
    //- Dissallow construction as copy
    procLduInterface(const procLduInterface&) = delete;
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const procLduInterface&);
};

}  // namespace mousse

#endif
