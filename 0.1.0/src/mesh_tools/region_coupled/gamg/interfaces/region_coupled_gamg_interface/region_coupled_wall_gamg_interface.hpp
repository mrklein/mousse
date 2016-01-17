// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledWallGAMGInterface
// Description
//   GAMG agglomerated coupled region interface.
// SourceFiles
//   region_coupled_wall_gamg_interface.cpp
#ifndef region_coupled_wall_gamg_interface_hpp_
#define region_coupled_wall_gamg_interface_hpp_
#include "region_coupled_base_gamg_interface.hpp"
namespace mousse
{
class regionCoupledWallGAMGInterface
:
  public regionCoupledBaseGAMGInterface
{
public:
  //- Runtime type information
  TYPE_NAME("regionCoupledWall");
  // Constructors
    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    regionCoupledWallGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );
    //- Disallow default bitwise copy construct
    regionCoupledWallGAMGInterface
    (
      const regionCoupledWallGAMGInterface&
    ) = delete;
    //- Disallow default bitwise assignment
    regionCoupledWallGAMGInterface& operator=
    (
      const regionCoupledWallGAMGInterface&
    ) = delete;
  //- Destructor
  virtual ~regionCoupledWallGAMGInterface();
    // I/O
      //- Write to stream
      virtual void write(Ostream&) const
      {
        //TBD. How to serialise the AMI such that we can stream
        // regionCoupledWallGAMGInterface.
        NOT_IMPLEMENTED
        (
          "regionCoupledWallGAMGInterface::write(Ostream&) const"
        );
      }
};
}  // namespace mousse
#endif
