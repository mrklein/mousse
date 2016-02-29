#ifndef EDGE_MESH_EDGE_MESH_FORMATS_EXTENDED_FEATURE_EDGE_MESH_FORMAT_HPP_
#define EDGE_MESH_EDGE_MESH_FORMATS_EXTENDED_FEATURE_EDGE_MESH_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::extendedFeatureEdgeMeshFormat
// Description
//   Provide a means of reading extendedFeatureEdgeMesh as featureEdgeMesh
// SourceFiles
//   extended_feature_edge_mesh_format.cpp
#include "edge_mesh.hpp"
namespace mousse
{
namespace fileFormats
{
class extendedFeatureEdgeMeshFormat
:
  public edgeMesh
{
public:
  // Constructors
    //- Construct from file name
    extendedFeatureEdgeMeshFormat(const fileName&);
    //- Disallow default bitwise copy construct
    extendedFeatureEdgeMeshFormat(const extendedFeatureEdgeMeshFormat&) = delete;
    //- Disallow default bitwise assignment
    extendedFeatureEdgeMeshFormat& operator=
    (
      const extendedFeatureEdgeMeshFormat&
    ) = delete;
  // Selectors
    //- Read file and return surface
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>
      (
        new extendedFeatureEdgeMeshFormat(name)
      );
    }
  //- Destructor
  virtual ~extendedFeatureEdgeMeshFormat()
  {}
  // Member Functions
    //- Read from file
    virtual bool read(const fileName&);
};
}  // namespace fileFormats
}  // namespace mousse
#endif
