// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::extendedFeatureEdgeMeshFormat
// Description
//   Provide a means of reading extendedFeatureEdgeMesh as featureEdgeMesh
// SourceFiles
//   extended_feature_edge_mesh_format.cpp
#ifndef extended_feature_edge_mesh_format_hpp_
#define extended_feature_edge_mesh_format_hpp_
#include "edge_mesh.hpp"
namespace mousse
{
namespace fileFormats
{
class extendedFeatureEdgeMeshFormat
:
  public edgeMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    extendedFeatureEdgeMeshFormat(const extendedFeatureEdgeMeshFormat&);
    //- Disallow default bitwise assignment
    void operator=(const extendedFeatureEdgeMeshFormat&);
public:
  // Constructors
    //- Construct from file name
    extendedFeatureEdgeMeshFormat(const fileName&);
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
