#ifndef MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_LDU_INTERFACE_REGION_COUPLED_LDU_INTERFACE_HPP_
#define MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_LDU_INTERFACE_REGION_COUPLED_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledLduInterface
// Description
//   An abstract base class for region coupled interfaces
// SourceFiles
//   region_coupled_ldu_interface.cpp
#include "primitive_fields_fwd.hpp"
#include "ami_patch_to_patch_interpolation.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
class regionCoupledLduInterface
{
public:
  //- Runtime type information
  TYPE_NAME("regionCoupledLduInterface");
  // Constructors
    //- Construct null
    regionCoupledLduInterface()
    {}
  //- Destructor
  virtual ~regionCoupledLduInterface();
  // Member Functions
    // Access
      //- Return neighbour
      virtual label neighbPatchID() const = 0;
      //- Is it owner?
      virtual bool owner() const = 0;
      //- Return neighb regionCoupledLduInterface
      virtual const regionCoupledLduInterface& neighbPatch() const = 0;
      //- Return AMI
      //virtual const AMIPatchToPatchInterpolation& AMI() const = 0;
      //- Return nbrMesh
      virtual const polyMesh& nbrMesh() const = 0;
};
}  // namespace mousse
#endif
