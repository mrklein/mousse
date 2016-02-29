#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_CYCLIC_GAMG_INTERFACE_FIELD_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_CYCLIC_GAMG_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicGAMGInterfaceField
// Description
//   GAMG agglomerated cyclic interface field.
// SourceFiles
//   cyclic_gamg_interface_field.cpp


#include "gamg_interface_field.hpp"
#include "cyclic_gamg_interface.hpp"
#include "cyclic_ldu_interface_field.hpp"

namespace mousse
{

class cyclicGAMGInterfaceField
:
  public GAMGInterfaceField,
  virtual public cyclicLduInterfaceField
{

  // Private data

    //- Local reference cast into the cyclic interface
    const cyclicGAMGInterface& cyclicInterface_;

    //- Is the transform required
    bool doTransform_;

    //- Rank of component for transformation
    int rank_;

public:

  //- Runtime type information
  TYPE_NAME("cyclic");

  // Constructors

    //- Construct from GAMG interface and fine level interface field
    cyclicGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterfaceField
    );

    //- Construct from GAMG interface and fine level interface field
    cyclicGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );

    //- Disallow default bitwise copy construct
    cyclicGAMGInterfaceField(const cyclicGAMGInterfaceField&) = delete;

    //- Disallow default bitwise assignment
    cyclicGAMGInterfaceField& operator=
    (
      const cyclicGAMGInterfaceField&
    ) = delete;

  //- Destructor
  virtual ~cyclicGAMGInterfaceField();

  // Member Functions

    // Access

      //- Return size
      label size() const
      {
        return cyclicInterface_.size();
      }

    //- Cyclic interface functions

      //- Does the interface field perform the transfromation
      virtual bool doTransform() const
      {
        return doTransform_;
      }

      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicInterface_.forwardT();
      }

      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicInterface_.reverseT();
      }

      //- Return rank of component for transform
      virtual int rank() const
      {
        return rank_;
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

};

}  // namespace mousse
#endif
