// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIGAMGInterfaceField
// Description
//   GAMG agglomerated cyclic interface field.
// SourceFiles
//   cyclic_amigamg_interface_field.cpp
#ifndef cyclic_amigamg_interface_field_hpp_
#define cyclic_amigamg_interface_field_hpp_
#include "gamg_interface_field.hpp"
#include "cyclic_amigamg_interface.hpp"
#include "cyclic_ami_ldu_interface_field.hpp"
namespace mousse
{
class cyclicAMIGAMGInterfaceField
:
  public GAMGInterfaceField,
  virtual public cyclicAMILduInterfaceField
{
  // Private data
    //- Local reference cast into the cyclic interface
    const cyclicAMIGAMGInterface& cyclicAMIInterface_;
    //- Is the transform required
    bool doTransform_;
    //- Rank of component for transformation
    int rank_;
public:
  //- Runtime type information
  TYPE_NAME("cyclicAMI");
  // Constructors
    //- Construct from GAMG interface and fine level interface field
    cyclicAMIGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterfaceField
    );
    //- Construct from GAMG interface and fine level interface field
    cyclicAMIGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );
    //- Disallow default bitwise copy construct
    cyclicAMIGAMGInterfaceField(const cyclicAMIGAMGInterfaceField&) = delete;
    //- Disallow default bitwise assignment
    cyclicAMIGAMGInterfaceField& operator=
    (
      const cyclicAMIGAMGInterfaceField&
    ) = delete;
  //- Destructor
  virtual ~cyclicAMIGAMGInterfaceField();
  // Member Functions
    // Access
      //- Return size
      label size() const
      {
        return cyclicAMIInterface_.size();
      }
    // Interface matrix update
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
    //- Cyclic interface functions
      //- Does the interface field perform the transfromation
      virtual bool doTransform() const
      {
        return doTransform_;
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicAMIInterface_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicAMIInterface_.reverseT();
      }
      //- Return rank of component for transform
      virtual int rank() const
      {
        return rank_;
      }
};
}  // namespace mousse
#endif
