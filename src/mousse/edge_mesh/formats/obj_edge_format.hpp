#ifndef EDGE_MESH_EDGE_MESH_FORMATS_OBJ_EDGE_FORMAT_HPP_
#define EDGE_MESH_EDGE_MESH_FORMATS_OBJ_EDGE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::OBJedgeFormat
// Description
//   Provide a means of reading/writing Alias/Wavefront OBJ format.
//   Does not handle negative vertex indices.

#include "edge_mesh.hpp"
#include "ifstream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"


namespace mousse {
namespace fileFormats {

class OBJedgeFormat
:
  public edgeMesh
{
  // Private Member Functions
    void readVertices
    (
      const string& line,
      string::size_type& endNum,
      DynamicList<label>& dynVertices
    );
public:
  // Constructors
    //- Construct from file name
    OBJedgeFormat(const fileName&);
    //- Disallow default bitwise copy construct
    OBJedgeFormat(const OBJedgeFormat&) = delete;
    //- Disallow default bitwise assignment
    OBJedgeFormat& operator=(const OBJedgeFormat&) = delete;
  // Selectors
    //- Read file and return surface
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>
      (
        new OBJedgeFormat(name)
      );
    }
  //- Destructor
  virtual ~OBJedgeFormat()
  {}
  // Member Functions
    //- Write surface mesh components by proxy
    static void write(const fileName&, const edgeMesh&);
    //- Read from file
    virtual bool read(const fileName&);
    //- Write object file
    virtual void write(const fileName& name) const
    {
      write(name, *this);
    }
};
}  // namespace fileFormats
}  // namespace mousse
#endif
