// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIGAMGInterfaceField
// Description
//   GAMG agglomerated cyclic interface for Arbitrarily Coupled Mesh Interface
//   (ACMI) fields.
// SourceFiles
//   cyclic_acmigamg_interface_field.cpp
#ifndef cyclic_acmigamg_interface_field_hpp_
#define cyclic_acmigamg_interface_field_hpp_
#include "gamg_interface_field.hpp"
#include "cyclic_acmigamg_interface.hpp"
#include "cyclic_acmi_ldu_interface_field.hpp"
namespace mousse
{
class cyclicACMIGAMGInterfaceField
:
  public GAMGInterfaceField,
  virtual public cyclicACMILduInterfaceField
{
  // Private data
    //- Local reference cast into the cyclic interface
    const cyclicACMIGAMGInterface& cyclicACMIInterface_;
    //- Is the transform required
    bool doTransform_;
    //- Rank of component for transformation
    int rank_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    cyclicACMIGAMGInterfaceField(const cyclicACMIGAMGInterfaceField&);
    //- Disallow default bitwise assignment
    void operator=(const cyclicACMIGAMGInterfaceField&);
public:
  //- Runtime type information
  TYPE_NAME("cyclicACMI");
  // Constructors
    //- Construct from GAMG interface and fine level interface field
    cyclicACMIGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterfaceField
    );
    //- Construct from GAMG interface and fine level interface field
    cyclicACMIGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );
  //- Destructor
  virtual ~cyclicACMIGAMGInterfaceField();
  // Member Functions
    // Access
      //- Return size
      label size() const
      {
        return cyclicACMIInterface_.size();
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
        return cyclicACMIInterface_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicACMIInterface_.reverseT();
      }
      //- Return rank of component for transform
      virtual int rank() const
      {
        return rank_;
      }
};
}  // namespace mousse
#endif
