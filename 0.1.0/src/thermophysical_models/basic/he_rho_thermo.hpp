// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heRhoThermo
// Description
//   Energy for a mixture based on density
// SourceFiles
//   he_rho_thermo.cpp
#ifndef he_rho_thermo_hpp_
#define he_rho_thermo_hpp_
#include "rho_thermo.hpp"
#include "he_thermo.hpp"
namespace mousse
{
template<class BasicPsiThermo, class MixtureType>
class heRhoThermo
:
  public heThermo<BasicPsiThermo, MixtureType>
{
  // Private Member Functions
    //- Calculate the thermo variables
    void calculate();
    //- Construct as copy (not implemented)
    heRhoThermo(const heRhoThermo<BasicPsiThermo, MixtureType>&);
public:
  //- Runtime type information
  TypeName("heRhoThermo");
  // Constructors
    //- Construct from mesh and phase name
    heRhoThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  //- Destructor
  virtual ~heRhoThermo();
  // Member functions
    //- Update properties
    virtual void correct();
};
}  // namespace mousse
#ifdef NoRepository
#   include "he_rho_thermo.cpp"
#endif
#endif
