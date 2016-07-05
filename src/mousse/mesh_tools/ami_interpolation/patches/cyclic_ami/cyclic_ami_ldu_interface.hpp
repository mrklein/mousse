#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_LDU_INTERFACE_FIELD_CYCLIC_AMI_LDU_INTERFACE_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_AMI_CYCLIC_AMI_LDU_INTERFACE_FIELD_CYCLIC_AMI_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMILduInterface
// Description
//   An abstract base class for cyclic AMI coupled interfaces

#include "primitive_fields_fwd.hpp"
#include "ami_patch_to_patch_interpolation.hpp"


namespace mousse {

class cyclicAMILduInterface
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicAMILduInterface");
  // Constructors
    //- Construct null
    cyclicAMILduInterface()
    {}
  //- Destructor
  virtual ~cyclicAMILduInterface();
  // Member Functions
    // Access
      //- Return neighbour
      virtual label neighbPatchID() const = 0;
      virtual bool owner() const = 0;
      //- Return processor number
      virtual const cyclicAMILduInterface& neighbPatch() const = 0;
      virtual const AMIPatchToPatchInterpolation& AMI() const = 0;
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const = 0;
      //- Return face reverse transformation tensor
      virtual const tensorField& reverseT() const = 0;
};

}  // namespace mousse
#endif
