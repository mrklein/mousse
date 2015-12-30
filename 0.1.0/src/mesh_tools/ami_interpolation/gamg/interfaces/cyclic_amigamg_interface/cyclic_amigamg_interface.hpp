// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIGAMGInterface
// Description
//   GAMG agglomerated cyclic AMI interface.
// SourceFiles
//   cyclic_amigamg_interface.cpp
#ifndef cyclic_amigamg_interface_hpp_
#define cyclic_amigamg_interface_hpp_
#include "gamg_interface.hpp"
#include "cyclic_ami_ldu_interface.hpp"
namespace mousse
{
class cyclicAMIGAMGInterface
:
  public GAMGInterface,
  virtual public cyclicAMILduInterface
{
  // Private data
    //- Reference for the cyclicLduInterface from which this is
    //  agglomerated
    const cyclicAMILduInterface& fineCyclicAMIInterface_;
    //- AMI interface
    autoPtr<AMIPatchToPatchInterpolation> amiPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    cyclicAMIGAMGInterface(const cyclicAMIGAMGInterface&);
    //- Disallow default bitwise assignment
    void operator=(const cyclicAMIGAMGInterface&);
public:
  //- Runtime type information
  TypeName("cyclicAMI");
  // Constructors
    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    cyclicAMIGAMGInterface
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
  virtual ~cyclicAMIGAMGInterface();
  // Member Functions
    // Interface transfer functions
      //- Transfer and return internal field adjacent to the interface
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;
    //- Cyclic interface functions
      //- Return neighbour processor number
      virtual label neighbPatchID() const
      {
        return fineCyclicAMIInterface_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return fineCyclicAMIInterface_.owner();
      }
      virtual const cyclicAMIGAMGInterface& neighbPatch() const
      {
        return dynamic_cast<const cyclicAMIGAMGInterface&>
        (
          coarseInterfaces_[neighbPatchID()]
        );
      }
      virtual const AMIPatchToPatchInterpolation& AMI() const
      {
        return amiPtr_();
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return fineCyclicAMIInterface_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return fineCyclicAMIInterface_.reverseT();
      }
    // I/O
      //- Write to stream
      virtual void write(Ostream&) const
      {
        //TBD. How to serialise the AMI such that we can stream
        // cyclicAMIGAMGInterface.
        notImplemented("cyclicAMIGAMGInterface::write(Ostream&) const");
      }
};
}  // namespace mousse
#endif
