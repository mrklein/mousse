// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::NASedgeFormat
// Description
//   Nastran edge reader.
// SourceFiles
//   nas_edge_format.cpp
#ifndef nas_edge_format_hpp_
#define nas_edge_format_hpp_
#include "edge_mesh.hpp"
#include "nas_core.hpp"
namespace mousse
{
namespace fileFormats
{
class NASedgeFormat
:
  public edgeMesh,
  public NASCore
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    NASedgeFormat(const NASedgeFormat&);
    //- Disallow default bitwise assignment
    void operator=(const NASedgeFormat&);
public:
  // Constructors
    //- Construct from file name
    NASedgeFormat(const fileName&);
  // Selectors
    //- Read file and return edge mesh
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>
      (
        new NASedgeFormat(name)
      );
    }
  //- Destructor
  virtual ~NASedgeFormat()
  {}
  // Member Functions
    //- Read from a file
    virtual bool read(const fileName&);
};
}  // namespace fileFormats
}  // namespace mousse
#endif
