// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_feature_edge_mesh.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(extendedFeatureEdgeMesh, 0);

}


// Constructors 
mousse::extendedFeatureEdgeMesh::extendedFeatureEdgeMesh(const IOobject& io)
:
  regIOobject{io},
  extendedEdgeMesh{}
{
  if (io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    if (readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
      WARNING_IN
      (
        "extendedFeatureEdgeMesh::extendedFeatureEdgeMesh"
        "(const IOobject&)"
      )
      << "Specified IOobject::MUST_READ_IF_MODIFIED but class"
      << " does not support automatic rereading."
      << endl;
    }
    readStream(typeName) >> *this;
    close();

    {
      // Calculate edgeDirections
      const edgeList& eds{edges()};
      const pointField& pts{points()};
      edgeDirections_.setSize(eds.size());
      FOR_ALL(eds, eI) {
        edgeDirections_[eI] = eds[eI].vec(pts);
      }
      edgeDirections_ /= (mag(edgeDirections_) + SMALL);
    }
  }
  if (debug) {
    Pout << "extendedFeatureEdgeMesh::extendedFeatureEdgeMesh :"
      << " constructed from IOobject :"
      << " points:" << points().size()
      << " edges:" << edges().size()
      << endl;
  }
}


mousse::extendedFeatureEdgeMesh::extendedFeatureEdgeMesh
(
  const IOobject& io,
  const extendedEdgeMesh& em
)
:
  regIOobject{io},
  extendedEdgeMesh{em}
{}


mousse::extendedFeatureEdgeMesh::extendedFeatureEdgeMesh
(
  const surfaceFeatures& sFeat,
  const objectRegistry& obr,
  const fileName& sFeatFileName,
  const boolList& surfBaffleRegions
)
:
  regIOobject
  {
    {
      sFeatFileName,
      obr.time().constant(),
      "extendedFeatureEdgeMesh",
      obr,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    }
  },
  extendedEdgeMesh{sFeat, surfBaffleRegions}
{}


mousse::extendedFeatureEdgeMesh::extendedFeatureEdgeMesh
(
  const IOobject& io,
  const PrimitivePatch<face, List, pointField, point>& surf,
  const labelList& featureEdges,
  const labelList& regionFeatureEdges,
  const labelList& featurePoints
)
:
  regIOobject{io},
  extendedEdgeMesh{surf, featureEdges, regionFeatureEdges, featurePoints}
{}


mousse::extendedFeatureEdgeMesh::extendedFeatureEdgeMesh
(
  const IOobject& io,
  const pointField& pts,
  const edgeList& eds,
  label concaveStart,
  label mixedStart,
  label nonFeatureStart,
  label internalStart,
  label flatStart,
  label openStart,
  label multipleStart,
  const vectorField& normals,
  const List<sideVolumeType>& normalVolumeTypes,
  const vectorField& edgeDirections,
  const labelListList& normalDirections,
  const labelListList& edgeNormals,
  const labelListList& featurePointNormals,
  const labelListList& featurePointEdges,
  const labelList& regionEdges
)
:
  regIOobject{io},
  extendedEdgeMesh
  {
    pts,
    eds,
    concaveStart,
    mixedStart,
    nonFeatureStart,
    internalStart,
    flatStart,
    openStart,
    multipleStart,
    normals,
    normalVolumeTypes,
    edgeDirections,
    normalDirections,
    edgeNormals,
    featurePointNormals,
    featurePointEdges,
    regionEdges
  }
{}


// Destructor 
mousse::extendedFeatureEdgeMesh::~extendedFeatureEdgeMesh()
{}


// Private Member Functions 
// Member Functions 
bool mousse::extendedFeatureEdgeMesh::readData(Istream& is)
{
  is >> *this;
  return !is.bad();
}


bool mousse::extendedFeatureEdgeMesh::writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}

