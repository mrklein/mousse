#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_LDU_INTERFACE_FIELD_CYCLIC_ACMI_LDU_INTERFACE_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_LDU_INTERFACE_FIELD_CYCLIC_ACMI_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMILduInterface
// Description
//   An abstract base class for cyclic ACMI coupled interfaces

#include "cyclic_ami_ldu_interface.hpp"


namespace mousse {

class cyclicACMILduInterface
:
  public cyclicAMILduInterface
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicACMILduInterface");
  // Constructors
    //- Construct null
    cyclicACMILduInterface()
    {}
  //- Destructor
  virtual ~cyclicACMILduInterface();
};

}  // namespace mousse

#endif
