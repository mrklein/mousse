// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::VTKedgeFormat
// Description
//   Provide a means of writing VTK legacy format.
// SourceFiles
//   vtk_edge_format.cpp
#ifndef vtk_edge_format_hpp_
#define vtk_edge_format_hpp_
#include "edge_mesh.hpp"
namespace mousse
{
namespace fileFormats
{
class VTKedgeFormat
:
  public edgeMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    VTKedgeFormat(const VTKedgeFormat&);
    //- Disallow default bitwise assignment
    void operator=(const VTKedgeFormat&);
protected:
  // Protected Member Functions
    //- Write header information with points
    static void writeHeader
    (
      Ostream&,
      const pointField&
    );
    //- Write edges
    static void writeEdges(Ostream&, const UList<edge>&);
public:
  // Constructors
    //- Construct from file name
    VTKedgeFormat(const fileName&);
  // Selectors
    //- Read file and return surface
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>
      (
        new VTKedgeFormat(name)
      );
    }
  //- Destructor
  virtual ~VTKedgeFormat()
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
