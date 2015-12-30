// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::surfaceFormatsCore
// Description
//   A collection of helper functions for reading/writing surface formats.
// SourceFiles
//   surface_formats_core.cpp
#ifndef surface_formats_core_hpp_
#define surface_formats_core_hpp_
#include "map.hpp"
#include "hash_set.hpp"
#include "label_list.hpp"
#include "surf_zone_list.hpp"
#include "surf_zone_identifier_list.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class IFstream;
class Time;
namespace fileFormats
{
class surfaceFormatsCore
{
protected:
    //- Return a list with a single entry,
    //  the size corresponds to that of the container
    template<class Container>
    static List<surfZone> oneZone
    (
      const Container& container,
      const word& name = "zone0"
    )
    {
      return List<surfZone>(1, surfZone(name, container.size(), 0, 0));
    }
    //- Read non-comment line
    static string getLineNoComment(IFstream&);
public:
  // Static Data
    //- The file extension corresponding to 'native' surface format
    //  Normally "ofs" (mnemonic: OF = OpenFOAM, S = Surface)
    static word nativeExt;
  // Static Member Functions
    static bool checkSupport
    (
      const wordHashSet& available,
      const word& ext,
      const bool verbose,
      const word& functionName
    );
    //- Return the local file name (within time directory)
    //  NEEDS FIXING
    static fileName localMeshFileName(const word& surfName="");
    //- Find instance with surfName
    //  NEEDS FIXING
    static fileName findMeshInstance(const Time&, const word& surfName="");
    //- Find mesh file with surfName
    //  NEEDS FIXING
    static fileName findMeshFile(const Time&, const word& surfName="");
  // Constructors
    //- Construct null
    surfaceFormatsCore();
  //- Destructor
  virtual ~surfaceFormatsCore();
};
}  // namespace fileFormats
}  // namespace mousse
#endif
