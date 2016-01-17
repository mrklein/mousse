// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::procLduInterface
// Description
//   IO interface for processorLduInterface
// SourceFiles
//   proc_ldu_interface.cpp
#ifndef proc_ldu_interface_hpp_
#define proc_ldu_interface_hpp_
#include "label_list.hpp"
#include "scalar_field.hpp"
namespace mousse
{
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
    //- Dissallow construction as copy
    procLduInterface(const procLduInterface&);
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
      return autoPtr<procLduInterface>(NULL);
    }
    static autoPtr<procLduInterface> New(Istream& is)
    {
      return autoPtr<procLduInterface>(new procLduInterface(is));
    }
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const procLduInterface&);
};
}  // namespace mousse
#endif
