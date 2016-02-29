#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_IO_MRF_ZONE_LIST_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_IO_MRF_ZONE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOMRFZoneList
// Description
//   List of MRF zones with IO functionality.  MRF zones are specified by a list
//   of dictionary entries, e.g.
//   \verbatim
//   zone1
//   {
//     cellZone    rotor1;
//     active      yes;
//     ...
//   }
//   zone2
//   {
//     cellZone    rotor2;
//     active      yes;
//     ...
//   }
//   \endverbatim
// SourceFiles
//   io_mrf_zone_list.cpp
#include "iodictionary.hpp"
#include "mrf_zone_list.hpp"
namespace mousse
{
class IOMRFZoneList
:
  public IOdictionary,
  public MRFZoneList
{
private:
  // Private Member Functions
    //- Create IO object if dictionary is present
    IOobject createIOobject(const fvMesh& mesh) const;
public:
  // Constructors
    //- Construct from mesh
    IOMRFZoneList(const fvMesh& mesh);
    //- Disallow default bitwise copy construct
    IOMRFZoneList(const IOMRFZoneList&) = delete;
    //- Disallow default bitwise assignment
    IOMRFZoneList& operator=(const IOMRFZoneList&) = delete;
    //- Destructor
    virtual ~IOMRFZoneList()
    {}
  // Member Functions
    //- Read dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
