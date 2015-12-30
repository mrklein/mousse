// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchPatchDist
// Description
//   Like wallDist but calculates on a patch the distance to nearest neighbouring
//   patches. Uses PatchEdgeFaceWave to do actual calculation.
// SourceFiles
//   patch_patch_dist.cpp
#ifndef patch_patch_dist_hpp_
#define patch_patch_dist_hpp_
#include "scalar_field.hpp"
#include "hash_set.hpp"
namespace mousse
{
//class polyMesh;
class polyPatch;
class patchPatchDist
:
  public scalarField
{
private:
  // Private Member Data
    //- Patch to operate on
    const polyPatch& patch_;
    //- Patches to determine the distance to
    const labelHashSet nbrPatchIDs_;
    //- Number of unset faces.
    label nUnset_;
public:
  // Constructors
    //- Construct from patch and neighbour patches.
    patchPatchDist
    (
      const polyPatch& pp,
      const labelHashSet& nbrPatchIDs
    );
  //- Destructor
  virtual ~patchPatchDist();
  // Member Functions
    const scalarField& y() const
    {
      return *this;
    }
    label nUnset() const
    {
      return nUnset_;
    }
    //- Correct for mesh geom/topo changes
    virtual void correct();
};
}  // namespace mousse
#endif
