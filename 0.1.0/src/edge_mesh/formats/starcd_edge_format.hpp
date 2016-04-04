#ifndef EDGE_MESH_EDGE_MESH_FORMATS_STARCD_EDGE_FORMAT_HPP_
#define EDGE_MESH_EDGE_MESH_FORMATS_STARCD_EDGE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STARCDedgeFormat
// Description
//   Read/write the lines from pro-STAR vrt/cel files.
// Note
//   Uses the extension \a .inp (input) to denote the format.
// See Also
//   mousse::meshReaders::STARCD

#include "edge_mesh.hpp"
#include "starcd_core.hpp"
#include "ifstream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"


namespace mousse {
namespace fileFormats {

class STARCDedgeFormat
:
  public edgeMesh,
  public STARCDCore
{
  // Private Data
    //- STAR-CD identifier for line shapes (1d elements)
    static const int starcdLineShape_ = 2;
    //- STAR-CD identifier for line type
    static const int starcdLineType_  = 5;
  // Private Member Functions
    static inline void writeLines
    (
      Ostream&,
      const edgeList&
    );
protected:
  // Protected Member Functions
  static void writeCase
  (
    Ostream&,
    const pointField&,
    const label nEdges
  );
public:
  // Constructors
    //- Construct from file name
    STARCDedgeFormat(const fileName&);
  // Selectors
    //- Read file and return edgeMesh
    static autoPtr<edgeMesh> New(const fileName& name)
    {
      return autoPtr<edgeMesh>{new STARCDedgeFormat{name}};
    }
    //- Disallow default bitwise copy construct
    STARCDedgeFormat(const STARCDedgeFormat&) = delete;
    //- Disallow default bitwise assignment
    STARCDedgeFormat& operator=(const STARCDedgeFormat&) = delete;
  //- Destructor
  virtual ~STARCDedgeFormat()
  {}
  // Member Functions
    //- Write edge mesh
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
