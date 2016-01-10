// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pisoControl
// Description
//   Specialization of the pimpleControl class for PISO control.
#ifndef piso_control_hpp_
#define piso_control_hpp_
#include "pimple_control.hpp"
namespace mousse
{
class pisoControl
:
  public pimpleControl
{
  // Private member functions
    //- Disallow default bitwise copy construct
    pisoControl(const pisoControl&);
    //- Disallow default bitwise assignment
    void operator=(const pisoControl&);
public:
  // Static Data Members
    //- Run-time type information
    TYPE_NAME("pisoControl");
  // Constructors
    //- Construct from mesh and the name of control sub-dictionary
    pisoControl(fvMesh& mesh, const word& dictName="PISO");
  //- Destructor
  virtual ~pisoControl();
};
}  // namespace mousse
#endif
