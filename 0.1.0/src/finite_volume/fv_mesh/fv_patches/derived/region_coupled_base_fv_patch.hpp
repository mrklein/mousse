#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_REGION_COUPLED_BASE_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_REGION_COUPLED_BASE_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledBaseFvPatch
// Description
//   Base class of regionCoupledFvPatch with common functionality for
//   regionCoupledFvPatch and regionCoupledWallFvPatch
// SourceFiles
//   region_coupled_base_fv_patch.cpp
#include "region_coupled_base.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "poly_patch.hpp"
#include "region_coupled_ldu_interface.hpp"
namespace mousse
{
class regionCoupledBaseFvPatch
:
  public regionCoupledLduInterface
{
protected:
  // Protected data
    //- Reference to regionCoupledBase
    const regionCoupledBase& regionCoupledBase_;
    //- Reference to fvPatch
    const fvPatch& patch_;
  // Protected members
    //- Returns fvMesh
    const fvMesh& nbrFvMesh() const
    {
      return
      (
        patch_.boundaryMesh().mesh().time().lookupObject<fvMesh>
        (
          regionCoupledBase_.nbrRegionName()
        )
      );
    }
public:
  //- Runtime type information
  TYPE_NAME("regionCoupledBase");
  // Constructors
    //- Construct from polyPatch
    regionCoupledBaseFvPatch
    (
      const polyPatch& pp,
      const fvPatch& patch
    )
    :
      regionCoupledLduInterface(),
      regionCoupledBase_
      (
        refCast<const regionCoupledBase>(pp)
      ),
      patch_(patch)
    {}
  //- Destructor
  virtual ~regionCoupledBaseFvPatch()
  {}
  // Member Functions
    // Access
      //- Return neighbour
      virtual label neighbPatchID() const
      {
        return regionCoupledBase_.neighbPatchID();
      }
      //- Is it the owner?
      virtual bool owner() const
      {
        return regionCoupledBase_.owner();
      }
      //- Return regionCoupledBase neighb Patch
      virtual const regionCoupledBaseFvPatch& neighbPatch() const
      {
        return refCast<const regionCoupledBaseFvPatch>
        (
          nbrFvMesh().boundary()
          [
            regionCoupledBase_.neighbPatchID()
          ]
        );
      }
      //- Return a reference to the AMI interpolator
      virtual const AMIPatchToPatchInterpolation& AMI() const
      {
        return regionCoupledBase_.AMI();
      }
      //- Returns neighbour polyMesh
      virtual const polyMesh& nbrMesh() const
      {
        return
        (
          patch_.boundaryMesh().mesh().time().lookupObject<polyMesh>
          (
            regionCoupledBase_.nbrRegionName()
          )
        );
      }
      //- Return fvPatch
      const fvPatch& patch() const
      {
        return patch_;
      }
      //- Returns if it is the same Region
      bool sameRegion() const
      {
        return regionCoupledBase_.sameRegion();
      }
      //- Return regionCoupledPolyPatch
      const regionCoupledBase& regionCoupledPatch() const
      {
        return regionCoupledBase_;
      }
      //- Return neighbor fvPatch
      const fvPatch& neighbFvPatch() const
      {
        return refCast<const fvPatch>
        (
          nbrFvMesh().boundary()
          [
            regionCoupledBase_.neighbPatchID()
          ]
        );
      }
      //- Return the interface type
      const word& regionCoupleType() const
      {
        return regionCoupledBase_.regionCoupleType();
      }
      //- Return faceCell addressing
      virtual const labelUList& faceCells() const = 0;
};
}  // namespace mousse
#endif
