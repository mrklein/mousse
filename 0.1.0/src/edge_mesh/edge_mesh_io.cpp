// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_mesh.hpp"
#include "bound_box.hpp"
#include "edge_mesh_format.hpp"


// Constructors 
mousse::edgeMesh::edgeMesh
(
  const fileName& name,
  const word& ext
)
:
  points_{0},
  edges_{0},
  pointEdgesPtr_{NULL}
{
  read(name, ext);
}


mousse::edgeMesh::edgeMesh(const fileName& name)
:
  points_{0},
  edges_{0},
  pointEdgesPtr_{NULL}
{
  read(name);
}


// Member Functions 
bool mousse::edgeMesh::read(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz") {
    fileName unzipName = name.lessExt();
    return read(unzipName, unzipName.ext());
  } else {
    return read(name, ext);
  }
}


// Read from file in given format
bool mousse::edgeMesh::read
(
  const fileName& name,
  const word& ext
)
{
  // read via selector mechanism
  transfer(New(name, ext)());
  return true;
}


void mousse::edgeMesh::write
(
  const fileName& name,
  const edgeMesh& mesh
)
{
  if (debug) {
    Info << "edgeMesh::write"
      "(const fileName&, const edgeMesh&) : "
      "writing to " << name
      << endl;
  }
  const word ext = name.ext();
  writefileExtensionMemberFunctionTable::iterator mfIter =
    writefileExtensionMemberFunctionTablePtr_->find(ext);
  if (mfIter == writefileExtensionMemberFunctionTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "MeshedSurface::write"
      "(const fileName&, const MeshedSurface&)"
    )
    << "Unknown file extension " << ext << nl << nl
    << "Valid types are :" << endl
    << writefileExtensionMemberFunctionTablePtr_->sortedToc()
    << exit(FatalError);
  } else {
    mfIter()(name, mesh);
  }
}


void mousse::edgeMesh::writeStats(Ostream& os) const
{
  os << indent << "points      : " << points().size() << nl;
  os << indent << "edges       : " << edges().size() << nl;
  os << indent << "boundingBox : " << boundBox(this->points()) << endl;
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const edgeMesh& em)
{
  fileFormats::edgeMeshFormat::write(os, em.points_, em.edges_);
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const edgeMesh&)");
  return os;
}


mousse::Istream& mousse::operator>>(Istream& is, edgeMesh& em)
{
  fileFormats::edgeMeshFormat::read(is, em.points_, em.edges_);
  em.pointEdgesPtr_.clear();
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, edgeMesh&)");
  return is;
}

