// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::greyMeanSolidAbsorptionEmission
// Description
//   greyMeanSolidAbsorptionEmission radiation absorption and emission
//   coefficients for continuous phase
//   The coefficients for the species in the Look up table have to be specified
//   for use in moles x P [atm], i.e. (k[i] = species[i]*p*9.869231e-6).
//   The coefficients for CO and soot or any other added are multiplied by the
//   respective mass fraction being solved
//   All the species in the dictionary need either to be in the look-up table or
//   being solved. Conversely, all the species solved do not need to be included
//   in the calculation of the absorption coefficient
//   The names of the species in the absorption dictionary must match exactly the
//   name in the look-up table or the name of the field being solved
// SourceFiles
//   grey_mean_solid_absorption_emission.cpp
#ifndef grey_mean_solid_absorption_emission_hpp_
#define grey_mean_solid_absorption_emission_hpp_
#include "absorption_emission_model.hpp"
#include "solid_thermo.hpp"
#include "basic_specie_mixture.hpp"
namespace mousse
{
namespace radiation
{
class greyMeanSolidAbsorptionEmission
:
  public absorptionEmissionModel
{
private:
  // Private data
    //- Enumering of radiative properties
    enum radiativeProperties
    {
      absorptivity,
      emissivity
    };
    //- Absorption model dictionary
    dictionary coeffsDict_;
    //- SLG thermo package
    const solidThermo& thermo_;
    //- Hash table of species names
    HashTable<label> speciesNames_;
    //- Basic multicomponent mixture
    const basicSpecieMixture& mixture_;
    //- List of solid species data
    List<FixedList<scalar, 2> > solidData_;
  // Private member functions
   //- Calculate the volumetric fraction of Yj
   tmp<scalarField> X(const word specie) const;
   //- Calculate the property mixing
   tmp<volScalarField> calc(const label) const;
public:
  //- Runtime type information
  TYPE_NAME("greyMeanSolidAbsorptionEmission");
  // Constructors
    //- Construct from components
    greyMeanSolidAbsorptionEmission
    (
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~greyMeanSolidAbsorptionEmission();
  // Member Functions
    // Access
      // Absorption coefficient
        //- Absorption coefficient for continuous phase
        tmp<volScalarField> aCont(const label bandI = 0) const;
      // Emission coefficient
        //- Emission coefficient for continuous phase
        tmp<volScalarField> eCont(const label bandI = 0) const;
  // Member Functions
    inline bool isGrey() const
    {
      return true;
    }
};
}  // namespace radiation
}  // namespace mousse
#endif
