// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_mesh.hpp"
#include "object_registry.hpp"
#include "processor_ldu_interface.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(lduMesh, 0);

}

// Member Functions
const mousse::objectRegistry& mousse::lduMesh::thisDb() const
{
  NOT_IMPLEMENTED("lduMesh::thisDb() const");
  const objectRegistry* orPtr_ = NULL;
  return *orPtr_;
}


// Friend Operators
mousse::Ostream& mousse::operator<<(Ostream& os, const InfoProxy<lduMesh>& ip)
{
  const lduMesh& ldum = ip.t_;
  const lduAddressing& addr = ldum.lduAddr();
  const lduInterfacePtrsList interfaces = ldum.interfaces();
  os << "lduMesh :"
    << " size:" << addr.size()
    << " l:" << addr.lowerAddr().size()
    << " u:" << addr.upperAddr().size()
    << " interfaces:" << interfaces.size()
    << " comm:" << ldum.comm()
    << endl;
  label nCouples = 0;
  FOR_ALL(interfaces, i) {
    if (interfaces.set(i)) {
      const labelUList& faceCells = addr.patchAddr(i);
      nCouples += faceCells.size();
      if (isA<processorLduInterface>(interfaces[i])) {
        const processorLduInterface& pi = refCast
        <
          const processorLduInterface
        >(interfaces[i]);
        os << "    patch:" << i
          << " type:" << interfaces[i].type()
          << " size:" << faceCells.size()
          << " myProcNo:" << pi.myProcNo()
          << " neighbProcNo:" << pi.neighbProcNo()
          << " comm:" << pi.comm()
          << endl;
      } else {
        os << "    patch:" << i
          << " type:" << interfaces[i].type()
          << " size:" << faceCells.size()
          << endl;
      }
    }
  }
  os << "    Interface faces/cells:" << scalar(nCouples)/addr.size() << endl;
  // Print actual contents
  if (lduMesh::debug) {
    const labelList& l = addr.lowerAddr();
    const labelList& u = addr.upperAddr();
    FOR_ALL(l, faceI) {
      os << "        face:" << faceI << " l:" << l[faceI]
        << " u:" << u[faceI] << endl;
    }
    FOR_ALL(interfaces, i) {
      if (interfaces.set(i)) {
        const labelUList& faceCells = addr.patchAddr(i);
        if (faceCells.size()) {
          os << "    patch:" << i
            << " type:" << interfaces[i].type() << endl;
          if (isA<processorLduInterface>(interfaces[i])) {
            const processorLduInterface& pi = refCast
            <
              const processorLduInterface
            >(interfaces[i]);
            os << "    myProcNo:" << pi.myProcNo()
              << " neighbProcNo:" << pi.neighbProcNo()
              << " comm:" << pi.comm()
              << endl;
          }
          FOR_ALL(faceCells, i) {
            os << "        " << i << " own:" << faceCells[i]
              << endl;
          }
        }
      }
    }
  }
  os.check("Ostream& operator<<(Ostream&, const lduMesh&");
  return os;
}
