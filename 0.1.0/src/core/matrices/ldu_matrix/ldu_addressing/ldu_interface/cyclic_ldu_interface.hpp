// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicLduInterface
// Description
//   An abstract base class for cyclic coupled interfaces.
// SourceFiles
//   cyclic_ldu_interface.cpp
#ifndef cyclic_ldu_interface_hpp_
#define cyclic_ldu_interface_hpp_
#include "ldu_interface.hpp"
#include "primitive_fields_fwd.hpp"
namespace mousse
{
class cyclicLduInterface
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicLduInterface");
  // Constructors
  //- Destructor
  virtual ~cyclicLduInterface();
  // Member Functions
    // Access
      //- Return neighbour
      virtual label neighbPatchID() const = 0;
      virtual bool owner() const = 0;
      //- Return processor number
      virtual const cyclicLduInterface& neighbPatch() const = 0;
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const = 0;
      //- Return face reverse transformation tensor
      virtual const tensorField& reverseT() const = 0;
};
}  // namespace mousse
#endif
