// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::edgeMeshFormat
// Description
//   Provide a means of reading/writing the single-file OpenFOAM edge format.
// Note
//  This class provides more methods than the regular edge format interface.
// SourceFiles
//   edge_mesh_format.cpp
#ifndef edge_mesh_format_hpp_
#define edge_mesh_format_hpp_
#include "edge_mesh.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
namespace mousse
{
namespace fileFormats
{
class edgeMeshFormat
:
  public edgeMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    edgeMeshFormat(const edgeMeshFormat&);
    //- Disallow default bitwise assignment
    void operator=(const edgeMeshFormat&);
protected:
  // Protected Member Functions
    //- Write header information
    static void writeHeader
    (
      Ostream&,
      const pointField&,
      const edgeList&
    );
public:
  // Constructors
    //- Construct from file name
    edgeMeshFormat(const fileName&);
  // Selectors
    //- Read file and return edgeMesh
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>
      (
        new edgeMeshFormat(name)
      );
    }
  //- Destructor
  virtual ~edgeMeshFormat()
  {}
  // Member Functions
    //- Read edgeMesh components from stream
    static bool read
    (
      Istream&,
      pointField&,
      edgeList&
    );
    //- Write edgeMesh components to stream
    static Ostream& write
    (
      Ostream&,
      const pointField&,
      const edgeList&
    );
    //- Write edgeMesh with a mimicked IOobject header
    static void write(const fileName&, const edgeMesh&);
    //- Read from file
    virtual bool read(const fileName&);
    //- Write object
    virtual void write(const fileName& name) const
    {
      write(name, *this);
    }
};
}  // namespace fileFormats
}  // namespace mousse
#endif
