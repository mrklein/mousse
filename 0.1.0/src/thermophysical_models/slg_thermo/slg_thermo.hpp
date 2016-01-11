// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SLGThermo
// Description
//   Thermo package for (S)olids (L)iquids and (G)ases
//   Takes reference to thermo package, and provides:
//   - carrier : components of thermo - access to elemental properties
//   - liquids : liquid components - access  to elemental properties
//   - solids  : solid components - access  to elemental properties
//   If thermo is not a multi-component thermo package, carrier is NULL.
//   Similarly, if no liquids or solids are specified, their respective
//   pointers will also be NULL.
//   Registered to the mesh so that it can be looked-up
// SourceFiles
//   slg_thermo.cpp
#ifndef slg_thermo_hpp_
#define slg_thermo_hpp_
#include "reg_ioobject.hpp"
#include "fluid_thermo.hpp"
#include "basic_specie_mixture.hpp"
#include "liquid_mixture_properties.hpp"
#include "solid_mixture_properties.hpp"
namespace mousse
{
class SLGThermo
:
  public regIOobject
{
  // Private data
    //- Thermo package
    fluidThermo& thermo_;
    //- Reference to the multi-component carrier phase thermo
    basicSpecieMixture* carrier_;
    //- Additional liquid properties data
    autoPtr<liquidMixtureProperties> liquids_;
    //- Additional solid properties data
    autoPtr<solidMixtureProperties> solids_;
public:
  //- Runtime type information
  TYPE_NAME("SLGThermo");
  // Constructors
    //- Construct from mesh
    SLGThermo(const fvMesh& mesh, fluidThermo& thermo);
  //- Destructor
  virtual ~SLGThermo();
  // Member Functions
    // Access
      //- Return reference to the thermo database
      const fluidThermo& thermo() const;
      //- Return reference to the gaseous components
      const basicSpecieMixture& carrier() const;
      //- Return reference to the global (additional) liquids
      const liquidMixtureProperties& liquids() const;
      //- Return reference to the global (additional) solids
      const solidMixtureProperties& solids() const;
      // Index retrieval
        //- Index of carrier component
        label carrierId
        (
          const word& cmptName,
          bool allowNotFound = false
        ) const;
        //- Index of liquid component
        label liquidId
        (
          const word& cmptName,
          bool allowNotFound = false
        ) const;
        //- Index of solid component
        label solidId
        (
          const word& cmptName,
          bool allowNotFound = false
        ) const;
    // Checks
      //- Thermo database has multi-component carrier flag
      bool hasMultiComponentCarrier() const;
      //- Thermo database has liquid components flag
      bool hasLiquids() const;
      //- Thermo database has solid components flag
      bool hasSolids() const;
    // IO
      bool writeData(mousse::Ostream&) const
      {
        return true;
      }
};
}  // namespace mousse
#endif
