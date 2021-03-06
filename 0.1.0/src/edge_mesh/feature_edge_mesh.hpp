#ifndef EDGE_MESH_FEATURE_EDGE_MESH_HPP_
#define EDGE_MESH_FEATURE_EDGE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::featureEdgeMesh
// Description
//   edgeMesh + IO.
// See Also
//   mousse::extendedFeatureEdgeMesh

#include "edge_mesh.hpp"
#include "reg_ioobject.hpp"


namespace mousse {

class featureEdgeMesh
:
  public regIOobject,
  public edgeMesh
{
public:
  TYPE_NAME("featureEdgeMesh");
  // Constructors
    //- Construct (read) given an IOobject
    featureEdgeMesh(const IOobject&);
    //- Construct from featureEdgeMesh data
    featureEdgeMesh
    (
      const IOobject&,
      const pointField&,
      const edgeList&
    );
    //- Construct as copy
    featureEdgeMesh(const IOobject&, const featureEdgeMesh&);
  // IO
    //- ReadData function required for regIOobject read operation
    virtual bool readData(Istream&);
    //- WriteData function required for regIOobject write operation
    virtual bool writeData(Ostream&) const;
};
}  // namespace mousse
#endif
