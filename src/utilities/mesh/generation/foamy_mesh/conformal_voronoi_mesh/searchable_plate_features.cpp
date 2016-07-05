// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_plate_features.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tree_bound_box.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(searchablePlateFeatures, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  searchableSurfaceFeatures,
  searchablePlateFeatures,
  dict
);

//! \cond - skip documentation : local scope only
const mousse::label edgesArray[4][2] =
{
  {0, 1}, // 0
  {0, 3},
  {2, 1}, // 2
  {2, 3}
};
//! \endcond

const edgeList searchablePlateFeatures::edges(calcEdges(edgesArray));

}


// Private Member Functions 
mousse::edgeList mousse::searchablePlateFeatures::calcEdges
(
  const label edgesArray[4][2]
)
{
  edgeList edges(4);
  FOR_ALL(edges, edgeI) {
    edges[edgeI][0] = edgesArray[edgeI][0];
    edges[edgeI][1] = edgesArray[edgeI][1];
  }
  return edges;
}


// Constructors 
mousse::searchablePlateFeatures::searchablePlateFeatures
(
  const searchableSurface& surface,
  const dictionary& dict
)
:
  searchableSurfaceFeatures{surface, dict},
  mode_
  {
    extendedFeatureEdgeMesh::sideVolumeTypeNames_
    [
      dict.lookupOrDefault<word>("meshableSide", "inside")
    ]
  }
{
  Info << indent
    << "    Meshable region = "
    << extendedFeatureEdgeMesh::sideVolumeTypeNames_[mode_]
    << endl;
}


// Destructor 
mousse::searchablePlateFeatures::~searchablePlateFeatures()
{}


// Member Functions 
mousse::autoPtr<mousse::extendedFeatureEdgeMesh>
mousse::searchablePlateFeatures::features() const
{
  autoPtr<extendedFeatureEdgeMesh> features;
  vectorField faceNormals{1};
  surface().getNormal(List<pointIndexHit>(1), faceNormals);
  vectorField edgeDirections{4};
  labelListList normalDirections{4};
  labelListList edgeNormals{4};
  FOR_ALL(edgeNormals, eI) {
    edgeNormals[eI].setSize(2, 0);
  }
  edgeNormals[0][0] = 0; edgeNormals[0][1] = 0;
  edgeNormals[1][0] = 0; edgeNormals[1][1] = 0;
  edgeNormals[2][0] = 0; edgeNormals[2][1] = 0;
  edgeNormals[3][0] = 0; edgeNormals[3][1] = 0;
  FOR_ALL(edgeDirections, eI) {
    edgeDirections[eI] =
      surface().points()()[edges[eI].end()]
      - surface().points()()[edges[eI].start()];
    normalDirections[eI] = labelList{2, label(0)};
    for (label j = 0; j < 2; ++j) {
      const vector cross =
        (faceNormals[edgeNormals[eI][j]] ^ edgeDirections[eI]);
      const vector fC0tofE0 =
        0.5*(max(surface().points()() + min(surface().points()())))
        - surface().points()()[edges[eI].start()];
      normalDirections[eI][j] =
        (
          (
            (cross/(mag(cross) + VSMALL)) & (fC0tofE0/(mag(fC0tofE0)+ VSMALL))
          ) > 0.0 ? 1 : -1
        );
    }
  }
  labelListList featurePointNormals{4};
  labelListList featurePointEdges{4};
  FOR_ALL(featurePointNormals, pI) {
    labelList& ftPtEdges = featurePointEdges[pI];
    ftPtEdges.setSize(2, 0);
    label edgeI = 0;
    FOR_ALL(edges, eI) {
      const edge& e = edges[eI];
      if (e.start() == pI) {
        ftPtEdges[edgeI++] = eI;
      } else if (e.end() == pI) {
        ftPtEdges[edgeI++] = eI;
      }
    }
    labelList& ftPtNormals = featurePointNormals[pI];
    ftPtNormals.setSize(1, 0);
    ftPtNormals[0] = edgeNormals[ftPtEdges[0]][0];
  }
  labelList regionEdges;
  features.set
  (
    new extendedFeatureEdgeMesh
    {
      IOobject
      {
        surface().name() + ".extendedFeatureEdgeMesh",
        surface().instance(),
        "extendedFeatureEdgeMesh",
        surface().db(),
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      surface().points(),
      edges,
      4, 4, 4,
      0, 0, 4, 4, // 4 flat edges
      faceNormals,
      List<extendedFeatureEdgeMesh::sideVolumeType>{4, mode_},
      edgeDirections,
      normalDirections,
      edgeNormals,
      featurePointNormals,
      featurePointEdges,
      regionEdges
    }
  );
  return features;
}

