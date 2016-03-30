// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_ldu_interface.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(processorLduInterface, 0);

}

// Private Member Functions
void mousse::processorLduInterface::resizeBuf
(
  List<char>& buf,
  const label size
) const
{
  if (buf.size() < size) {
    buf.setSize(size);
  }
}


// Constructors
mousse::processorLduInterface::processorLduInterface()
:
  sendBuf_{0},
  receiveBuf_{0}
{}


// Destructor
mousse::processorLduInterface::~processorLduInterface()
{}
