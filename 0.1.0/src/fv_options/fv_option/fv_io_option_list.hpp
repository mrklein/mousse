// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::IOoptionList
// Description
//   IOoptionList
// SourceFiles
//   fv_io_option_list.cpp
#ifndef fv_io_option_list_hpp_
#define fv_io_option_list_hpp_
#include "fv_option_list.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
namespace fv
{
class IOoptionList
:
  public IOdictionary,
  public optionList
{
private:
  // Private Member Functions
    //- Create IO object if dictionary is present
    IOobject createIOobject(const fvMesh& mesh) const;
    //- Disallow default bitwise copy construct
    IOoptionList(const IOoptionList&);
    //- Disallow default bitwise assignment
    void operator=(const IOoptionList&);
public:
  // Constructors
    //- Construct from components with list of field names
    IOoptionList(const fvMesh& mesh);
  //- Destructor
  virtual ~IOoptionList()
  {}
  // Member Functions
    //- Inherit read from optionList
    using optionList::read;
    //- Read dictionary
    virtual bool read();
};
}  // namespace fv
}  // namespace mousse
#endif
