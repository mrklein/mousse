// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledGAMGInterfaceField
// Description
//   GAMG agglomerated region coupled interface field.
// SourceFiles
//   region_coupled_gamg_interface_field.cpp
#ifndef region_coupled_gamg_interface_field_hpp_
#define region_coupled_gamg_interface_field_hpp_
#include "gamg_interface_field.hpp"
#include "region_coupled_gamg_interface.hpp"
namespace mousse
{
class regionCoupledGAMGInterfaceField
:
  public GAMGInterfaceField
{
  // Private data
    //- Local reference cast into the cyclic interface
    const regionCoupledGAMGInterface& regionCoupledGAMGInterface_;
  // Private Member Functions
public:
  //- Runtime type information
  TYPE_NAME("regionCoupled");
  // Constructors
    //- Construct from GAMG interface and fine level interface field
    regionCoupledGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterfaceField
    );
    //- Construct from GAMG interface and fine level interface field
    regionCoupledGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );
    //- Disallow default bitwise copy construct
    regionCoupledGAMGInterfaceField
    (
      const regionCoupledGAMGInterfaceField&
    ) = delete;
    //- Disallow default bitwise assignment
    regionCoupledGAMGInterfaceField& operator=
    (
      const regionCoupledGAMGInterfaceField&
    ) = delete;
  //- Destructor
  virtual ~regionCoupledGAMGInterfaceField();
  // Member Functions
    // Access
      //- Return size
      label size() const
      {
        return regionCoupledGAMGInterface_.size();
      }
      //- Interface matrix update
      virtual void updateInterfaceMatrix
      (
        scalarField&,
        const scalarField&,
        const scalarField&,
        const direction,
        const Pstream::commsTypes
      ) const
      {}
};
}  // namespace mousse
#endif
