// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMILduInterface
// Description
//   An abstract base class for cyclic ACMI coupled interfaces
// SourceFiles
//   cyclic_acmi_ldu_interface.cpp
#ifndef cyclic_acmi_ldu_interface_hpp_
#define cyclic_acmi_ldu_interface_hpp_
#include "cyclic_ami_ldu_interface.hpp"
namespace mousse
{
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
