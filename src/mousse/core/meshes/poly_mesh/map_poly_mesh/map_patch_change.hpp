#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_PATCH_CHANGE_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_PATCH_CHANGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapPatchChange
// Description
//   Class containing mesh-to-mesh mapping information after a patch change
//   operation.

#include "label_list.hpp"


namespace mousse {

class mapPatchChange
{
  // Private data

    //- Old patches
    const label nOldPatches_;

    //- Patch mapping array
    const labelList patchMap_;

public:

  // Constructors

    //- Construct from components
    mapPatchChange(const label nOldPatches, const labelList& patchMap)
    :
      nOldPatches_{nOldPatches},
      patchMap_{patchMap}
    {}

  // Member Functions

    // Access

      //- Number of old patches
      label nOldPatches() const
      {
        return nOldPatches_;
      }

      //- Patch map. Size of current patches.
      //  -1  : patch was added
      //  >=0 : old position of patch
      //  any original patch which is not in the list has been deleted
      const labelList& patchMap() const
      {
        return patchMap_;
      }

    // Utility functions

      //- Labels of added patches
      labelList addedPatches() const
      {
        labelList added{patchMap_.size()};
        label addedI = 0;
        FOR_ALL(patchMap_, patchI)
        {
          if (patchMap_[patchI] == -1)
          {
            added[addedI++] = patchI;
          }
        }
        added.setSize(addedI);
        return added;
      }

      //- Labels (on old mesh) of deleted patches
      labelList deletedPatches() const
      {
        labelList oldToNew(nOldPatches_, -1);
        // Mark all preserved patches
        FOR_ALL(patchMap_, patchI)
        {
          if (patchMap_[patchI] != -1)
          {
            oldToNew[patchMap_[patchI]] = patchI;
          }
        }
        // Extract -1 elements from oldToNew. These are the deleted
        // patches.
        label deletedI = 0;
        FOR_ALL(oldToNew, oldPatchI)
        {
          if (oldToNew[oldPatchI] == -1)
          {
            oldToNew[deletedI++] = oldPatchI;
          }
        }
        oldToNew.setSize(deletedI);
        return oldToNew;
      }
};

}  // namespace mousse
#endif
