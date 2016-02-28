// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsources::dummy
// Description
// SourceFiles
//   dummy.cpp
#ifndef dummy_hpp_
#define dummy_hpp_
#include "iat_esource.hpp"
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
class dummy
:
  public IATEsource
{
public:
  //- Runtime type information
  TypeName("dummy");
  // Constructors
    dummy
    (
      const word& name,
      const IATE& iate,
      const dictionary& dict
    )
    :
      IATEsource(iate)
    {}
  //- Destructor
  virtual ~dummy()
  {}
  // Member Functions
    virtual tmp<volScalarField> R() const;
};
}  // namespace IATEsources
}  // namespace diameterModels
}  // namespace mousse
#endif
