#ifndef MESH_TOOLS_CELL_FEATURES_CELL_FEATURES_HPP_
#define MESH_TOOLS_CELL_FEATURES_CELL_FEATURES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellFeatures
// Description
//   Cell analysis class.
//   Constructs feature edges and feature points, which are edges/points with
//   and angle > given specification.
//   Can be asked for 'superFaces' which can be used to see if a cell is a
//   'splitHex'.
// SourceFiles
//   cell_features.cpp
#include "face_list.hpp"
#include "label_list.hpp"
#include "bool_list.hpp"
#include "hash_set.hpp"
#include "map.hpp"
#include "dynamic_list.hpp"
namespace mousse
{
// Forward declaration of classes
class primitiveMesh;
class cellFeatures
{
  // Private data
    const primitiveMesh& mesh_;
    //- Cos of angle between two connected faces or two connected edges on
    //  same face before edge/point is 'feature'.
    scalar minCos_;
    label cellI_;
    //- Feature edges
    labelHashSet featureEdge_;
    //- (demand driven) Faces after removing internal points&edges
    mutable faceList* facesPtr_;
    //- New to old face mapping
    mutable List<DynamicList<label> > faceMap_;
  // Private Member Functions
    bool faceAlignedEdge(const label, const label) const;
    label nextEdge
    (
      const Map<label>& toSuperFace,
      const label superFaceI,
      const label thisEdgeI,
      const label thisVertI
    ) const;
    bool isCellFeatureEdge(const scalar, const label) const;
    void walkSuperFace
    (
      const label faceI,
      const label superFaceI,
      Map<label>& toSuperFace
    ) const;
    void calcSuperFaces() const;
public:
  // Constructors
    //- Construct from cell in mesh
    cellFeatures
    (
      const primitiveMesh&,
      const scalar minCos,    // angle to use for feature recognition.
      const label cellI
    );
    //- Disallow default bitwise copy construct
    cellFeatures(const cellFeatures&) = delete;
    //- Disallow default bitwise assignment
    cellFeatures& operator=(const cellFeatures&) = delete;
  //- Destructor
  ~cellFeatures();
  // Member Functions
    // Access
      const labelHashSet& featureEdge() const
      {
        return featureEdge_;
      }
      const faceList& faces() const
      {
        if (!facesPtr_)
        {
          calcSuperFaces();
        }
        return *facesPtr_;
      }
      //- New to old faceMap. Guaranteed to be shrunk.
      const List<DynamicList<label> >& faceMap() const
      {
        if (!facesPtr_)
        {
          calcSuperFaces();
        }
        return faceMap_;
      }
    // Check
      //- Is edge a feature edge (uniquely determined since on cell
      //  only two faces sharing edge)
      bool isFeatureEdge(const label edgeI) const
      {
        return featureEdge().found(edgeI);
      }
      //- Are two edges connected at feature point?
      //  Is local to face since point might be seen as feature point
      //  from one face but not from another.
      bool isFeaturePoint(const label edge0, const label edge1) const;
      //- Is vertexI on faceI used by two edges that form feature
      //  point
      bool isFeatureVertex(const label faceI, const label vertI) const;
};
}  // namespace mousse
#endif
