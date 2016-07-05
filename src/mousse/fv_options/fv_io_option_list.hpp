#ifndef FV_OPTIONS_FV_OPTION_FV_IO_OPTION_LIST_HPP_
#define FV_OPTIONS_FV_OPTION_FV_IO_OPTION_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::IOoptionList
// Description
//   IOoptionList

#include "fv_option_list.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"


namespace mousse {
namespace fv {

class IOoptionList
:
  public IOdictionary,
  public optionList
{
private:
  // Private Member Functions
    //- Create IO object if dictionary is present
    IOobject createIOobject(const fvMesh& mesh) const;
public:
  // Constructors
    //- Construct from components with list of field names
    IOoptionList(const fvMesh& mesh);
    //- Disallow default bitwise copy construct
    IOoptionList(const IOoptionList&) = delete;
    //- Disallow default bitwise assignment
    IOoptionList& operator=(const IOoptionList&) = delete;
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

