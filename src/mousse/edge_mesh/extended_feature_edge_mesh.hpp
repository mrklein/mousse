#ifndef EDGE_MESH_EXTENDED_FEATURE_EDGE_MESH_HPP_
#define EDGE_MESH_EXTENDED_FEATURE_EDGE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedFeatureEdgeMesh
// Description
//   extendedEdgeMesh + IO.

#include "extended_edge_mesh.hpp"
#include "reg_ioobject.hpp"


namespace mousse {

class objectRegistry;


class extendedFeatureEdgeMesh
:
  public regIOobject,
  public extendedEdgeMesh
{
public:
  //- Runtime type information
  TYPE_NAME("extendedFeatureEdgeMesh");
  // Constructors
    //- Construct (read) given an IOobject
    extendedFeatureEdgeMesh(const IOobject&);
    //- Construct as copy
    extendedFeatureEdgeMesh
    (
      const IOobject&,
      const extendedEdgeMesh&
    );
    //- Construct given a surface with selected edges,point
    //  (surfaceFeatures), an objectRegistry and a
    //  fileName to write to.
    //  Extracts, classifies and reorders the data from surfaceFeatures.
    extendedFeatureEdgeMesh
    (
      const surfaceFeatures& sFeat,
      const objectRegistry& obr,
      const fileName& sFeatFileName,
      const boolList& surfBaffleRegions
    );
    //- Construct from PrimitivePatch
    extendedFeatureEdgeMesh
    (
      const IOobject&,
      const PrimitivePatch<face, List, pointField, point>& surf,
      const labelList& featureEdges,
      const labelList& regionFeatureEdges,
      const labelList& featurePoints
    );
    //- Construct from all components
    extendedFeatureEdgeMesh
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
    );
  //- Destructor
  virtual ~extendedFeatureEdgeMesh();
  // IO
    //- Give precedence to the regIOobject write
    using regIOobject::write;
    //- ReadData function required for regIOobject read operation
    virtual bool readData(Istream&);
    //- WriteData function required for regIOobject write operation
    virtual bool writeData(Ostream&) const;
};
}  // namespace mousse
#endif
