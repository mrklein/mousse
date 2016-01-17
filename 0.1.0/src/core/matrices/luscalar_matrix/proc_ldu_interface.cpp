// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "proc_ldu_interface.hpp"
#include "ldu_interface_field.hpp"
#include "cyclic_ldu_interface.hpp"
#include "processor_ldu_interface.hpp"
// Constructors 
mousse::procLduInterface::procLduInterface
(
  const lduInterfaceField& interface,
  const scalarField& coeffs
)
:
  faceCells_(interface.interface().faceCells()),
  coeffs_(coeffs),
  myProcNo_(-1),
  neighbProcNo_(-1),
  tag_(-1),
  comm_(-1)
{
  if (isA<processorLduInterface>(interface.interface()))
  {
    const processorLduInterface& pldui =
      refCast<const processorLduInterface>(interface.interface());
    myProcNo_ = pldui.myProcNo();
    neighbProcNo_ = pldui.neighbProcNo();
    tag_ = pldui.tag();
    comm_ = pldui.comm();
  }
  else if (isA<cyclicLduInterface>(interface.interface()))
  {
  }
  else
  {
    FATAL_ERROR_IN
    (
      "procLduInterface::procLduInterface"
      "(const lduInterfaceField&, const scalarField&"
    )   << "Unknown lduInterface type "
      << interface.interface().type()
      << exit(FatalError);
  }
}
mousse::procLduInterface::procLduInterface(Istream& is)
:
  faceCells_(is),
  coeffs_(is),
  myProcNo_(readLabel(is)),
  neighbProcNo_(readLabel(is)),
  tag_(readLabel(is)),
  comm_(readLabel(is))
{}
// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const procLduInterface& cldui)
{
  os  << cldui.faceCells_
    << cldui.coeffs_
    << cldui.myProcNo_
    << cldui.neighbProcNo_
    << cldui.tag_
    << cldui.comm_;
  return os;
}
