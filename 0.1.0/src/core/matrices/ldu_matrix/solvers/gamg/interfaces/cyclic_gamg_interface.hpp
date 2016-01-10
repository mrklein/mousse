// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicGAMGInterface
// Description
//   GAMG agglomerated cyclic interface.
// SourceFiles
//   cyclic_gamg_interface.cpp

#ifndef cyclic_gamg_interface_hpp_
#define cyclic_gamg_interface_hpp_

#include "gamg_interface.hpp"
#include "cyclic_ldu_interface.hpp"

namespace mousse
{

class cyclicGAMGInterface
:
  public GAMGInterface,
  virtual public cyclicLduInterface
{
  // Private data

    //- Neigbour patch number
    label neighbPatchID_;

    //- Am I owner?
    bool owner_;

    //- Transformation tensor
    tensorField forwardT_;

    //- Transformation tensor
    tensorField reverseT_;

public:

  //- Runtime type information
  TYPE_NAME("cyclic");

  // Constructors

    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    cyclicGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );

    //- Construct from Istream
    cyclicGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      Istream& is
    );

    //- Disallow default bitwise copy construct
    cyclicGAMGInterface(const cyclicGAMGInterface&) = delete;

    //- Disallow default bitwise assignment
    cyclicGAMGInterface& operator=(const cyclicGAMGInterface&) = delete;

  //- Destructor
  virtual ~cyclicGAMGInterface();

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
        return neighbPatchID_;
      }

      virtual bool owner() const
      {
        return owner_;
      }

      virtual const cyclicGAMGInterface& neighbPatch() const
      {
        return dynamic_cast<const cyclicGAMGInterface&>
        (
          coarseInterfaces_[neighbPatchID()]
        );
      }

      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return forwardT_;
      }

      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return reverseT_;
      }

    // I/O

      //- Write to stream
      virtual void write(Ostream&) const;

};

}  // namespace mousse
#endif
