// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledFvPatch
// Description
//   Common functionality for regionCoupleFvPatch and regionCoupledWallFvPatch
// SourceFiles
//   region_coupled_fv_patch.cpp
#ifndef region_coupled_fv_patch_hpp_
#define region_coupled_fv_patch_hpp_
#include "fv_patch.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "region_coupled_poly_patch.hpp"
#include "region_coupled_base_fv_patch.hpp"
namespace mousse
{
class regionCoupledFvPatch
:
  public lduInterface,
  public fvPatch,
  public regionCoupledBaseFvPatch
{
  // Private data
    const regionCoupledPolyPatch& regionCoupledPolyPatch_;
  // Private members
    //- Return regionCoupledFvPatch nbr
    const regionCoupledFvPatch& neighbFvPatch() const
    {
      return refCast<const regionCoupledFvPatch>
      (
        nbrFvMesh().boundary()
        [
          neighbPatchID()
        ]
      );
    }
public:
  //- Runtime type information
  TYPE_NAME(regionCoupledPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    regionCoupledFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm),
      regionCoupledBaseFvPatch
      (
        patch,
        *this
      ),
      regionCoupledPolyPatch_
      (
        refCast<const regionCoupledPolyPatch>(patch)
      )
    {}
  //- Destructor
  ~regionCoupledFvPatch()
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
