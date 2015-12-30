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
#ifndef extended_edge_mesh_format_hpp_
#define extended_edge_mesh_format_hpp_
#include "extended_edge_mesh.hpp"
namespace mousse
{
namespace fileFormats
{
class extendedEdgeMeshFormat
:
  public extendedEdgeMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    extendedEdgeMeshFormat(const extendedEdgeMeshFormat&);
    //- Disallow default bitwise assignment
    void operator=(const extendedEdgeMeshFormat&);
public:
  // Constructors
    //- Construct from file name
    extendedEdgeMeshFormat(const fileName&);
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
