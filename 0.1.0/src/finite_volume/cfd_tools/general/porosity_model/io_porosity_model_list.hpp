#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_POROSITY_MODEL_IO_POROSITY_MODEL_LIST_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_POROSITY_MODEL_IO_POROSITY_MODEL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOporosityModelList
// Description
//   List of porosity models with IO functionality
// SourceFiles
//   io_porosity_model_list.cpp
#include "iodictionary.hpp"
#include "porosity_model_list.hpp"
namespace mousse
{
class IOporosityModelList
:
  public IOdictionary,
  public porosityModelList
{
private:
  // Private Member Functions
    //- Create IO object if dictionary is present
    IOobject createIOobject(const fvMesh& mesh) const;
public:
  // Constructors
    //- Construct from mesh
    IOporosityModelList(const fvMesh& mesh);
    //- Disallow default bitwise copy construct
    IOporosityModelList(const IOporosityModelList&) = delete;
    //- Disallow default bitwise assignment
    IOporosityModelList& operator=(const IOporosityModelList&) = delete;
    //- Destructor
    virtual ~IOporosityModelList()
    {}
  // Member Functions
    //- Read dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
