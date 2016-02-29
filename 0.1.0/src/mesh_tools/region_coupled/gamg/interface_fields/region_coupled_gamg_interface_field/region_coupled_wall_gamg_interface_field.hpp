#ifndef MESH_TOOLS_REGION_COUPLED_GAMG_INTERFACE_FIELDS_REGION_COUPLED_GAMG_INTERFACE_FIELD_REGION_COUPLED_WALL_GAMG_INTERFACE_FIELD_HPP_
#define MESH_TOOLS_REGION_COUPLED_GAMG_INTERFACE_FIELDS_REGION_COUPLED_GAMG_INTERFACE_FIELD_REGION_COUPLED_WALL_GAMG_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledWallGAMGInterfaceField
// Description
//   GAMG agglomerated region coupled interface field.
// SourceFiles
//   region_coupled_wall_gamg_interface_field.cpp
#include "gamg_interface_field.hpp"
#include "region_coupled_wall_gamg_interface.hpp"
namespace mousse
{
class regionCoupledWallGAMGInterfaceField
:
  public GAMGInterfaceField
{
  // Private data
    //- Local reference cast into the region coupled interface
    const regionCoupledWallGAMGInterface& regionCoupledGAMGInterface_;
public:
  //- Runtime type information
  TYPE_NAME("regionCoupledWall");
  // Constructors
    //- Construct from GAMG interface and fine level interface field
    regionCoupledWallGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterfaceField
    );
    //- Construct from GAMG interface and fine level interface field
    regionCoupledWallGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );
    //- Disallow default bitwise copy construct
    regionCoupledWallGAMGInterfaceField
    (
      const regionCoupledWallGAMGInterfaceField&
    ) = delete;
    //- Disallow default bitwise assignment
    regionCoupledWallGAMGInterfaceField& operator=
    (
      const regionCoupledWallGAMGInterfaceField&
    ) = delete;
  //- Destructor
  virtual ~regionCoupledWallGAMGInterfaceField();
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
