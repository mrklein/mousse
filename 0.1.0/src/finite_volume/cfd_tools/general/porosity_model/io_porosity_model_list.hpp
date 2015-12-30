// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOporosityModelList
// Description
//   List of porosity models with IO functionality
// SourceFiles
//   io_porosity_model_list.cpp
#ifndef io_porosity_model_list_hpp_
#define io_porosity_model_list_hpp_
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
    //- Disallow default bitwise copy construct
    IOporosityModelList(const IOporosityModelList&);
    //- Disallow default bitwise assignment
    void operator=(const IOporosityModelList&);
public:
  // Constructors
    //- Construct from mesh
    IOporosityModelList(const fvMesh& mesh);
    //- Destructor
    virtual ~IOporosityModelList()
    {}
  // Member Functions
    //- Read dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
