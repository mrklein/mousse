#ifndef CORE_MESHES_LDU_MESH_HPP_
#define CORE_MESHES_LDU_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lduMesh
// Description
//   Abstract base class for meshes which provide LDU addressing for the
//   construction of lduMatrix and LDU-solvers.

#include "ldu_addressing.hpp"
#include "ldu_interface_ptrs_list.hpp"
#include "type_info.hpp"
#include "info_proxy.hpp"


namespace mousse {

class objectRegistry;
class lduMesh
{
public:
  //- Runtime type information
  TYPE_NAME("lduMesh");
  // Constructors
  //- Destructor
  virtual ~lduMesh()
  {}
  // Member Functions
    // Access
      //- Return the object registry
      virtual const objectRegistry& thisDb() const;
      //- Return ldu addressing
      virtual const lduAddressing& lduAddr() const = 0;
      //- Return a list of pointers for each patch
      //  with only those pointing to interfaces being set
      virtual lduInterfacePtrsList interfaces() const = 0;
      //- Return communicator used for parallel communication
      virtual label comm() const = 0;
      //- Helper: reduce with current communicator
      template<class T, class BinaryOp>
      void reduce
      (
        T& Value,
        const BinaryOp& bop
      ) const;
    // Info
      //- Return info proxy.
      //  Used to print mesh information to a stream
      InfoProxy<lduMesh> info() const
      {
        return *this;
      }
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const InfoProxy<lduMesh>&);
};

}  // namespace mousse

#include "ldu_mesh.ipp"

#endif
