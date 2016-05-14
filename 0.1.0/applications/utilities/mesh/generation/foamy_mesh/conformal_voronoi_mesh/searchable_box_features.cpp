// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_box_features.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tree_bound_box.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(searchableBoxFeatures, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  searchableSurfaceFeatures,
  searchableBoxFeatures,
  dict
);

}


// Constructors 
mousse::searchableBoxFeatures::searchableBoxFeatures
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
mousse::searchableBoxFeatures::~searchableBoxFeatures()
{}


// Member Functions 
mousse::autoPtr<mousse::extendedFeatureEdgeMesh>
mousse::searchableBoxFeatures::features() const
{
  autoPtr<extendedFeatureEdgeMesh> features;
  List<vector> faceNormalsList{treeBoundBox::faceNormals};
  vectorField faceNormals{faceNormalsList};
  vectorField edgeDirections{12};
  labelListList normalDirections{12};
  labelListList edgeNormals{12};
  FOR_ALL(edgeNormals, eI) {
    edgeNormals[eI].setSize(2, 0);
  }
  edgeNormals[0][0] = 2; edgeNormals[0][1] = 4;
  edgeNormals[1][0] = 1; edgeNormals[1][1] = 4;
  edgeNormals[2][0] = 3; edgeNormals[2][1] = 4;
  edgeNormals[3][0] = 0; edgeNormals[3][1] = 4;
  edgeNormals[4][0] = 2; edgeNormals[4][1] = 5;
  edgeNormals[5][0] = 1; edgeNormals[5][1] = 5;
  edgeNormals[6][0] = 3; edgeNormals[6][1] = 5;
  edgeNormals[7][0] = 0; edgeNormals[7][1] = 5;
  edgeNormals[8][0] = 0; edgeNormals[8][1] = 2;
  edgeNormals[9][0] = 2; edgeNormals[9][1] = 1;
  edgeNormals[10][0] = 1; edgeNormals[10][1] = 3;
  edgeNormals[11][0] = 3; edgeNormals[11][1] = 0;
  tmp<pointField> surfacePointsTmp{surface().points()};
  auto& surfacePoints = surfacePointsTmp();
  FOR_ALL(edgeDirections, eI) {
    edgeDirections[eI] =
      surfacePoints[treeBoundBox::edges[eI].end()]
      - surfacePoints[treeBoundBox::edges[eI].start()];
    normalDirections[eI] = labelList{2, label(0)};
    for (label j = 0; j < 2; ++j) {
      const vector cross =
        (faceNormals[edgeNormals[eI][j]] ^ edgeDirections[eI]);
      const vector fC0tofE0 =
        0.5*(max(surfacePoints + min(surfacePoints)))
        - surfacePoints[treeBoundBox::edges[eI].start()];
      normalDirections[eI][j] =
        (
          (
            (cross/(mag(cross) + VSMALL)) & (fC0tofE0/(mag(fC0tofE0)+ VSMALL))
          ) > 0.0 ? 1 : -1
        );
    }
  }
  labelListList featurePointNormals{8};
  labelListList featurePointEdges{8};
  FOR_ALL(featurePointNormals, pI) {
    labelList& ftPtEdges = featurePointEdges[pI];
    ftPtEdges.setSize(3, 0);
    label edgeI = 0;
    FOR_ALL(treeBoundBox::edges, eI) {
      const edge& e = treeBoundBox::edges[eI];
      if (e.start() == pI) {
        ftPtEdges[edgeI++] = eI;
      } else if (e.end() == pI) {
        ftPtEdges[edgeI++] = eI;
      }
    }
    labelList& ftPtNormals = featurePointNormals[pI];
    ftPtNormals.setSize(3, 0);
    ftPtNormals[0] = edgeNormals[ftPtEdges[0]][0];
    ftPtNormals[1] = edgeNormals[ftPtEdges[0]][1];
    ftPtNormals[2] = edgeNormals[ftPtEdges[1]][0];
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
      treeBoundBox::edges,
      8, 8, 8,
      12, 12, 12, 12,
      faceNormals,
      List<extendedFeatureEdgeMesh::sideVolumeType>{12, mode_},
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

