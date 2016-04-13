#ifndef EDGE_MESH_EDGE_MESH_FORMATS_EDGE_MESH_FORMATS_CORE_HPP_
#define EDGE_MESH_EDGE_MESH_FORMATS_EDGE_MESH_FORMATS_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::edgeMeshFormatsCore
// Description
//   A collection of helper functions for reading/writing edge formats.

#include "map.hpp"
#include "hash_set.hpp"
#include "label_list.hpp"


namespace mousse {

// Forward declaration of classes
class IFstream;
class Time;

namespace fileFormats {

class edgeMeshFormatsCore
{
protected:
  // Protected Member Functions
    //- Read non-comment line
    static string getLineNoComment(IFstream&);
public:
  // Static Data
    //- The file extension corresponding to 'native' edge format
    //  Normally "eMesh" (edge-mesh)
    static word nativeExt;
  // Static Member Functions
    static bool checkSupport
    (
      const wordHashSet& available,
      const word& ext,
      const bool verbose,
      const word& functionName
    );
   // //- Return the local file name (within time directory)
   // //  NEEDS FIXING
   // static fileName localMeshFileName(const word& edgeName="");
   // //- Find instance with edgeName
   // //  NEEDS FIXING
   // static fileName findMeshInstance
   // (
   //     const Time&,
   //     const word& edgeName=""
   // );
   // //- Find mesh file with edgeName
   // //  NEEDS FIXING
   // static fileName findMeshFile(const Time&, const word& edgeName="");
  // Constructors
    //- Construct null
    edgeMeshFormatsCore();
  //- Destructor
  virtual ~edgeMeshFormatsCore();
};
}  // namespace fileFormats
}  // namespace mousse
#endif
