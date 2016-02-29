#ifndef EDGE_MESH_EXTENDED_EDGE_MESH_FORMAT_HPP_
#define EDGE_MESH_EXTENDED_EDGE_MESH_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::extendedEdgeMeshFormat
// Description
//   Provide a means of reading/writing the single-file OpenFOAM
//   extendedEdgeMesh format
// SourceFiles
//   extended_edge_mesh_format.cpp
#include "extended_edge_mesh.hpp"
namespace mousse
{
namespace fileFormats
{
class extendedEdgeMeshFormat
:
  public extendedEdgeMesh
{
public:
  // Constructors
    //- Construct from file name
    extendedEdgeMeshFormat(const fileName&);
    //- Disallow default bitwise copy construct
    extendedEdgeMeshFormat(const extendedEdgeMeshFormat&) = delete;
    //- Disallow default bitwise assignment
    extendedEdgeMeshFormat& operator=(const extendedEdgeMeshFormat&) = delete;
  //- Destructor
  virtual ~extendedEdgeMeshFormat()
  {}
  // Member Functions
    //- Read from file
    virtual bool read(const fileName&);
};
}  // namespace fileFormats
}  // namespace mousse
#endif
