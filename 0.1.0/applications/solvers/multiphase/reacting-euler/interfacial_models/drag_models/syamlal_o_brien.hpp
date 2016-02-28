// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::SyamlalOBrien
// Description
//   Syamlal, M., Rogers, W. and O'Brien, T. J. (1993) MFIX documentation,
//   Theory Guide. Technical Note DOE/METC-94/1004. Morgantown, West Virginia,
//   USA.
// SourceFiles
//   syamlal_o_brien.cpp
#ifndef syamlal_o_brien_hpp_
#define syamlal_o_brien_hpp_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class SyamlalOBrien
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("SyamlalOBrien");
  // Constructors
    //- Construct from a dictionary and a phase pair
    SyamlalOBrien
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~SyamlalOBrien();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
