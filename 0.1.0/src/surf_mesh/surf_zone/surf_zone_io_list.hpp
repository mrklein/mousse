// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfZoneIOList
// Description
//   IOobject for a surfZoneList
// SourceFiles
//   surf_zone_io_list.cpp
#ifndef surf_zone_io_list_hpp_
#define surf_zone_io_list_hpp_
#include "surf_zone_list.hpp"
#include "reg_ioobject.hpp"
#include "face_list.hpp"
#include "class_name.hpp"
namespace mousse
{
// Forward declaration of classes
class surfZoneIOList
:
  public surfZoneList,
  public regIOobject
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    surfZoneIOList(const surfZoneIOList&);
    //- Disallow default bitwise assignment
    void operator=(const surfZoneIOList&);
public:
  //- Runtime type information
  TypeName("surfZoneList");
  // Constructors
    //- Construct from IOobject
    explicit surfZoneIOList(const IOobject&);
    //- Construct from IOobject and surfZoneList
    surfZoneIOList(const IOobject&, const surfZoneList&);
    //- Construct from IOobject and surfZoneList
    surfZoneIOList(const IOobject&, const Xfer<surfZoneList>&);
  //- Destructor
  ~surfZoneIOList();
  // Member Functions
    //- writeData member function required by regIOobject
    bool writeData(Ostream&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const surfZoneIOList&);
};
}  // namespace mousse
#endif
