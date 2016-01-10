// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledGAMGInterface
// Description
//   GAMG agglomerated coupled region interface.
// SourceFiles
//   region_coupled_gamg_interface.cpp
#ifndef region_coupled_gamg_interface_hpp_
#define region_coupled_gamg_interface_hpp_
#include "region_coupled_base_gamg_interface.hpp"
namespace mousse
{
class regionCoupledGAMGInterface
:
  public regionCoupledBaseGAMGInterface
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    regionCoupledGAMGInterface(const regionCoupledGAMGInterface&);
    //- Disallow default bitwise assignment
    void operator=(const regionCoupledGAMGInterface&);
public:
  //- Runtime type information
  TYPE_NAME("regionCoupled");
  // Constructors
    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    regionCoupledGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );
  //- Destructor
  virtual ~regionCoupledGAMGInterface();
    // I/O
      //- Write to stream
      virtual void write(Ostream&) const
      {
        //TBD. How to serialise the AMI such that we can stream
        // regionCoupledGAMGInterface.
        NOT_IMPLEMENTED
        (
          "regionCoupledGAMGInterface::write(Ostream&) const"
        );
      }
};
}  // namespace mousse
#endif
