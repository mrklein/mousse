// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GAMGInterfaceField
// Description
//   Abstract base class for GAMG agglomerated interface fields.
// SourceFiles
//   gamg_interface_field.cpp
//   gamg_interface_field_new.cpp

#ifndef gamg_interface_field_hpp_
#define gamg_interface_field_hpp_

#include "ldu_interface_field.hpp"
#include "gamg_interface.hpp"
#include "auto_ptr.hpp"

namespace mousse
{
class GAMGInterfaceField
:
  public lduInterfaceField
{
  // Private data

    //- Local reference cast into the interface
    const GAMGInterface& interface_;

public:

  //- Runtime type information
  TYPE_NAME("GAMGInterfaceField");

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      GAMGInterfaceField,
      lduInterfaceField,
      (
        const GAMGInterface& GAMGCp,
        const lduInterfaceField& fineInterface
      ),
      (GAMGCp, fineInterface)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      GAMGInterfaceField,
      lduInterface,
      (
        const GAMGInterface& GAMGCp,
        const bool doTransform,
        const int rank
      ),
      (GAMGCp, doTransform, rank)
    );

  // Selectors

    //- Return a pointer to a new interface created on freestore given
    //  the fine interface
    static autoPtr<GAMGInterfaceField> New
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterface
    );

    //- Return a pointer to a new interface created on freestore given
    //  the fine interface
    static autoPtr<GAMGInterfaceField> New
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );

  // Constructors

    //- Construct from GAMG interface and fine level interface field
    GAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField&
    )
    :
      lduInterfaceField{GAMGCp},
      interface_{GAMGCp}
    {}

    //- Construct from GAMG interface and fine level interface field
    GAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool /*doTransform*/,
      const int /*rank*/
    )
    :
      lduInterfaceField{GAMGCp},
      interface_{GAMGCp}
    {}

    //- Disallow default bitwise copy construct
    GAMGInterfaceField(const GAMGInterfaceField&) = delete;

    //- Disallow default bitwise assignment
    GAMGInterfaceField& operator=(const GAMGInterfaceField&) = delete;

  // Member Functions

    // Access

      //- Return interface
      const GAMGInterface& interface() const
      {
        return interface_;
      }
};
}  // namespace mousse
#endif
