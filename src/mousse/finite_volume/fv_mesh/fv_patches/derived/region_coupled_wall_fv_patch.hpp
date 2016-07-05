#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_REGION_COUPLED_WALL_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_REGION_COUPLED_WALL_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledWallFvPatch
// Description
//   mousse::regionCoupledWallFvPatch
// SourceFiles
//   region_coupled_wall_fv_patch.cpp
#include "wall_fv_patch.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "region_coupled_wall_poly_patch.hpp"
#include "region_coupled_base_fv_patch.hpp"
namespace mousse
{
class regionCoupledWallFvPatch
:
  public lduInterface,
  public wallFvPatch,
  public regionCoupledBaseFvPatch
{
  // Private data
    //- Const reference to regionCoupledWallPolyPatch
    const regionCoupledWallPolyPatch& regionCoupledPolyPatch_;
  // Private members
    //- Return regionCoupledBaseFvPatch nbr
    const regionCoupledWallFvPatch& neighbFvPatch() const
    {
      return refCast<const regionCoupledWallFvPatch>
      (
        nbrFvMesh().boundary()[neighbPatchID()]
      );
    }
public:
  //- Runtime type information
  TYPE_NAME(regionCoupledWallPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    regionCoupledWallFvPatch
    (
      const polyPatch& patch,
      const fvBoundaryMesh& bm
    )
    :
      wallFvPatch(patch, bm),
      regionCoupledBaseFvPatch
      (
        patch,
        *this
      ),
      regionCoupledPolyPatch_
      (
        refCast<const regionCoupledWallPolyPatch>(patch)
      )
    {}
  //- Destructor
  ~regionCoupledWallFvPatch()
  {}
  // Member Functions
    // Access
      //- Return faceCell addressing
      virtual const labelUList& faceCells() const
      {
        return fvPatch::faceCells();
      }
      //- Return true because this patch is coupled
      virtual bool coupled() const
      {
        return regionCoupledPolyPatch_.coupled();
      }
    // Interface transfer functions
      //- Return the values of the given internal data adjacent to
      //  the interface as a field
      virtual tmp<labelField> interfaceInternalField
      (
        const labelUList& internalData
      ) const;
      //- Inherit initInternalFieldTransfer from lduInterface
      using lduInterface::initInternalFieldTransfer;
      //- Initialise neighbour field transfer
      virtual void initInternalFieldTransfer
      (
        const Pstream::commsTypes,
        labelUList& /*iF*/
      ) const
      {}
      //- Return neighbour field
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;
};
}  // namespace mousse
#endif
