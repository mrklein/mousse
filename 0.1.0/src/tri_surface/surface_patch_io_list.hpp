// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfacePatchIOList
// Description
//   IOobject for a surfacePatchList
// SourceFiles
//   surface_patch_io_list.cpp
#ifndef surface_patch_io_list_hpp_
#define surface_patch_io_list_hpp_
#include "surface_patch_list.hpp"
#include "reg_ioobject.hpp"
#include "face_list.hpp"
#include "class_name.hpp"
namespace mousse
{
// Forward declaration of classes
class surfacePatchIOList
:
  public surfacePatchList,
  public regIOobject
{
  // Private data
  // Private Member Functions
    //- Disallow default bitwise copy construct
    surfacePatchIOList(const surfacePatchIOList&);
    //- Disallow default bitwise assignment
    void operator=(const surfacePatchIOList&);
public:
  //- Runtime type information
  TYPE_NAME("surfacePatchIOList");
  // Static data members
    //- Static data someStaticData
  // Constructors
    //- Construct from IOobject
    explicit surfacePatchIOList(const IOobject& io);
    //- Construct from IOobject
    surfacePatchIOList(const IOobject& io, const surfacePatchList&);
  //- Destructor
  ~surfacePatchIOList();
  // Member Functions
    //- writeData member function required by regIOobject
    bool writeData(Ostream&) const;
  // Member Operators
  // Friend Functions
  // Friend Operators
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const surfacePatchIOList&);
};
}  // namespace mousse
#endif
