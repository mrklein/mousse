// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIGAMGInterface
// Description
//   GAMG agglomerated cyclic ACMI interface.
// SourceFiles
//   cyclic_acmigamg_interface.cpp
#ifndef cyclic_acmigamg_interface_hpp_
#define cyclic_acmigamg_interface_hpp_
#include "gamg_interface.hpp"
#include "cyclic_acmi_ldu_interface.hpp"
namespace mousse
{
class cyclicACMIGAMGInterface
:
  public GAMGInterface,
  virtual public cyclicACMILduInterface
{
  // Private data
    //- Reference for the cyclicLduInterface from which this is
    //  agglomerated
    const cyclicACMILduInterface& fineCyclicACMIInterface_;
    //- AMI interface
    autoPtr<AMIPatchToPatchInterpolation> amiPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    cyclicACMIGAMGInterface(const cyclicACMIGAMGInterface&);
    //- Disallow default bitwise assignment
    void operator=(const cyclicACMIGAMGInterface&);
public:
  //- Runtime type information
  TypeName("cyclicACMI");
  // Constructors
    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    cyclicACMIGAMGInterface
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
  virtual ~cyclicACMIGAMGInterface();
  // Member Functions
    // Interface transfer functions
      //- Transfer and return internal field adjacent to the interface
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;
    //- Cyclic interface functions
      //- Return neigbour processor number
      virtual label neighbPatchID() const
      {
        return fineCyclicACMIInterface_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return fineCyclicACMIInterface_.owner();
      }
      virtual const cyclicACMIGAMGInterface& neighbPatch() const
      {
        return dynamic_cast<const cyclicACMIGAMGInterface&>
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
        return fineCyclicACMIInterface_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return fineCyclicACMIInterface_.reverseT();
      }
    // I/O
      //- Write to stream
      virtual void write(Ostream&) const
      {
        //TBD. How to serialise the AMI such that we can stream
        // cyclicACMIGAMGInterface.
        notImplemented
        (
          "cyclicACMIGAMGInterface::write(Ostream&) const"
        );
      }
};
}  // namespace mousse
#endif
