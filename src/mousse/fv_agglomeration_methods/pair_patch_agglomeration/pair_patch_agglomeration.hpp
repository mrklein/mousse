#ifndef FV_AGGLOMERATION_METHODS_PAIR_PATCH_AGGLOMERATION_PAIR_PATCH_AGGLOMERATION_HPP_
#define FV_AGGLOMERATION_METHODS_PAIR_PATCH_AGGLOMERATION_PAIR_PATCH_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPatchAgglomeration
// Description
//   Primitive patch pair agglomerate method.

#include "mathematical_constants.hpp"
#include "poly_patch.hpp"
#include "indirect_primitive_patch.hpp"
#include "list.hpp"
#include "edge_map.hpp"


namespace mousse {

typedef PrimitivePatch<face, List, const pointField> bPatch;

class pairPatchAgglomeration
{
protected:
  // Protected data
    //- Number of levels to merge, 1 = don't merge, 2 = merge pairs etc.
    label mergeLevels_;
    //- Max number of levels
    label maxLevels_;
    //- Number of faces in coarsest level
    label nFacesInCoarsestLevel_;
    //- Feature angle
    scalar featureAngle_;
    //- The number of faces in each level
    labelList nFaces_;
    //- Cell restriction addressing array.
    //  Maps from the finer to coarse level
    PtrList<labelField> restrictAddressing_;
    //- Maps from finest to coarsest
    labelList restrictTopBottomAddressing_;
    //- Hierarchy of patch addressing
    PtrList<bPatch> patchLevels_;
    //- Edge weights
    EdgeMap<scalar> facePairWeight_;
private:
  // Private Member Functions
    //- Assemble coarse patch
    bool agglomeratePatch
    (
      const bPatch& patch,
      const labelList& fineToCoarse,
      const label fineLevelIndex
    );
    //- Agglomerate one level
    tmp<labelField> agglomerateOneLevel
    (
      label& nCoarseCells,
      const bPatch& patchLevel
    );
    //- Combine leves
    void combineLevels(const label curLevel);
    //- Shrink the number of levels to that specified
    void compactLevels(const label fineLevelIndex);
    //- Check the need for further agglomeration
    bool continueAgglomerating(const label fineLevelIndex);
    //- Set edge weights
    void setEdgeWeights(const label indexLevel);
    //- Set base patch edge weights
    void setBasedEdgeWeights();
    //- Maps current level with base patch
    void mapBaseToTopAgglom(const label fineLevelIndex);
public:
  // Constructors
    //- Construct given mesh and controls
    pairPatchAgglomeration
    (
      const polyPatch& patch,
      const dictionary& controlDict,
      const bool additionalWeights = false
    );
    //- Disallow default bitwise copy construct
    pairPatchAgglomeration(const pairPatchAgglomeration&) = delete;
    //- Disallow default bitwise assignment
    pairPatchAgglomeration& operator=(const pairPatchAgglomeration&) = delete;
  // Destructor
  ~pairPatchAgglomeration();
  // Member Functions
    //- Agglomerate patch
    void agglomerate();
    // Access
      //- Return size
      label size() const { return patchLevels_.size(); }
      //- Return restriction from top level to bottom level
      const labelList& restrictTopBottomAddressing() const
      {
        return restrictTopBottomAddressing_;
      }
      //- Return primitivePatch of given level
      const bPatch& patchLevel(const label leveli) const;
      //- Return cell restrict addressing of given level
      const labelField& restrictAddressing(const label leveli) const
      {
        return restrictAddressing_[leveli];
      }
    // Restriction and prolongation
      //- Restrict (integrate by summation) cell field
      template<class Type>
      void restrictField
      (
        Field<Type>& cf,
        const Field<Type>& ff,
        const label fineLevelIndex
      ) const;
      //- Prolong (interpolate by injection) cell field
      template<class Type>
      void prolongField
      (
        Field<Type>& ff,
        const Field<Type>& cf,
        const label coarseLevelIndex
      ) const;
};

}  // namespace mousse

#include "pair_patch_agglomeration.ipp"

#endif