#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_REFINEMENT_FEATURES_REFINEMENT_FEATURES_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_REFINEMENT_FEATURES_REFINEMENT_FEATURES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementFeatures
// Description
//   Encapsulates queries for features.
// SourceFiles
//   refinement_features.cpp
#include "extended_feature_edge_mesh.hpp"
#include "indexed_octree.hpp"
#include "tree_data_edge.hpp"
#include "tree_data_point.hpp"
namespace mousse
{
class refinementFeatures
:
  public PtrList<extendedFeatureEdgeMesh>
{
private:
  // Private data
    //- Per shell the list of ranges
    List<scalarField> distances_;
    //- Per shell per distance the refinement level
    labelListList levels_;
    //- Edge
    PtrList<indexedOctree<treeDataEdge> > edgeTrees_;
    //- Features points
    PtrList<indexedOctree<treeDataPoint> > pointTrees_;
    //- Region edge trees (demand driven)
    mutable autoPtr<PtrList<indexedOctree<treeDataEdge> > >
      regionEdgeTreesPtr_;
  // Private Member Functions
    //- Read set of feature edge meshes
    void read(const objectRegistry&, const PtrList<dictionary>&);
    //- Build edge tree and feature point tree
    void buildTrees(const label);
    //- Find shell level higher than ptLevel
    void findHigherLevel
    (
      const pointField& pt,
      const label featI,
      labelList& maxLevel
    ) const;
protected:
    const PtrList<indexedOctree<treeDataEdge> >& edgeTrees() const
    {
      return edgeTrees_;
    }
    const PtrList<indexedOctree<treeDataPoint> >& pointTrees() const
    {
      return pointTrees_;
    }
    const PtrList<indexedOctree<treeDataEdge> >& regionEdgeTrees() const;
public:
  // Constructors
    //- Construct from description
    refinementFeatures
    (
      const objectRegistry& io,
      const PtrList<dictionary>& featDicts
    );
  // Member Functions
    // Access
      //- Per featureEdgeMesh the list of level
      const labelListList& levels() const
      {
        return levels_;
      }
      //- Per featureEdgeMesh the list of ranges
      const List<scalarField>& distances() const
      {
        return distances_;
      }
    // Query
      //- Highest distance of all features
      scalar maxDistance() const;
      //- Find nearest point on nearest feature edge. Sets
      //  - nearFeature: index of feature mesh
      //  - nearInfo   : location on feature edge and edge index
      //                 (note: not feature edge index but index into
      //                  edges() directly)
      //  - nearNormal : local feature edge normal
      void findNearestEdge
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& nearFeature,
        List<pointIndexHit>& nearInfo,
        vectorField& nearNormal
      ) const;
      //- Find nearest point on nearest region edge. Sets
      //  - nearFeature: index of feature mesh
      //  - nearInfo   : location on feature edge and edge index
      //                 (note: not feature edge index but index into
      //                  edges() directly)
      //  - nearNormal : local feature edge normal
      void findNearestRegionEdge
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& nearFeature,
        List<pointIndexHit>& nearInfo,
        vectorField& nearNormal
      ) const;
      //- Find nearest feature point. Sets
      //  - nearFeature: index of feature mesh
      //  - nearInfo   : location on feature point and point index.
      //                 (note: not index into shapes().pointLabels() but
      //                  index into points() directly)
      void findNearestPoint
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& nearFeature,
        List<pointIndexHit>& nearInfo
      ) const;
      //- Find shell level higher than ptLevel
      void findHigherLevel
      (
        const pointField& pt,
        const labelList& ptLevel,
        labelList& maxLevel
      ) const;
};
}  // namespace mousse
#endif
