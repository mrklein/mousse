// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::topoCellLooper
// Description
//   Implementation of cellLooper. This one recognizes splitHexes and tries
//   to make a cut such that if the neighbour was split (in a previous iteration)
//   this one also gets split in the same direction so that the result
//   will be a mesh without splitHexes.
//   'splitHexes' are cells of which the 'featureEdges'
//   (see cellFeatures class) form a hex. The remaining non-feature edges
//   are assumed to result from splitting the neighbour and this class tries
//   to start from one of these and cut through to an opposite edge.
//   The current set of cuts (vertIsCut, edgeIsCut, edgeWeight) are not being
//   used by this implementation.
//   All non-splitHexes are done by the parent classes.
// SourceFiles
//   topo_cell_looper.cpp
#ifndef topo_cell_looper_hpp_
#define topo_cell_looper_hpp_
#include "hex_cell_looper.hpp"
#include "type_info.hpp"
namespace mousse
{
// Forward declaration of classes
class cellFeatures;
class topoCellLooper
:
  public hexCellLooper
{
  // Private Member Functions
    //- In-memory truncate a list
    template<class T>
    static void subsetList
    (
      const label startI,
      const label freeI,
      DynamicList<T>& lst
    );
    //- Walk across superface discarding non-feature points.
    void walkFace
    (
      const cellFeatures& features,
      const label faceI,
      const label startEdgeI,
      const label startVertI,
      const label nFeaturePts,
      label& edgeI,
      label& vertI
    ) const;
    //- Returns list of vertices on 'superEdge' i.e. list of edges connected
    // by non-feature points. First and last are feature points, ones
    // inbetween are not.
    labelList getSuperEdge
    (
      const cellFeatures& features,
      const label faceI,
      const label startEdgeI,
      const label startVertI
    ) const;
    // Return non-feature edge from cells' edges that is most
    // perpendicular to refinement direction. Used as starting edge.
    label getAlignedNonFeatureEdge
    (
      const vector& refDir,
      const label cellI,
      const cellFeatures& features
    ) const;
    //- Starts from edge and vertex on edge on face (or neighbouring face)
    // and steps either to existing vertex (vertI != -1) or to edge
    // (vertI == -1)
    // by walking point-edge and crossing nFeats featurePoints.
    void walkAcrossFace
    (
      const cellFeatures& features,
      const label faceI,
      const label startEdgeI,
      const label startVertI,
      const label nFeats,
      label& edgeI,
      label& vertI
    ) const;
    //- Walks splitcell circumference. Sets loop/loopweights to walk on
    //  outside of cell.
    void walkSplitHex
    (
      const label cellI,
      const cellFeatures& features,
      const label fromFaceI,
      const label fromEdgeI,
      const label fromVertI,
      DynamicList<label>& loop,
      DynamicList<scalar>& loopWeights
    ) const;
    //- Disallow default bitwise copy construct
    topoCellLooper(const topoCellLooper&);
    //- Disallow default bitwise assignment
    void operator=(const topoCellLooper&);
public:
  //- Runtime type information
  TypeName("topoCellLooper");
  // Static data members
    //- Cos of angle for feature recognition (of splitHexes)
    static const scalar featureCos;
  // Constructors
    //- Construct from components
    topoCellLooper(const polyMesh& mesh);
  //- Destructor
  virtual ~topoCellLooper();
  // Member Functions
    //- Create cut along circumference of cellI. Gets current mesh cuts.
    //  Cut along circumference is expressed as loop of cuts plus weights
    //  for cuts along edges (only valid for edge cuts).
    //  Return true if successful cut.
    virtual bool cut
    (
      const vector& refDir,
      const label cellI,
      const boolList& vertIsCut,
      const boolList& edgeIsCut,
      const scalarField& edgeWeight,
      labelList& loop,
      scalarField& loopWeights
    ) const;
    //- Same but now also base point of cut provided (instead of always
    //  cell centre)
    virtual bool cut
    (
      const plane& cutPlane,
      const label cellI,
      const boolList& vertIsCut,
      const boolList& edgeIsCut,
      const scalarField& edgeWeight,
      labelList& loop,
      scalarField& loopWeights
    ) const;
};
}  // namespace mousse
#endif
