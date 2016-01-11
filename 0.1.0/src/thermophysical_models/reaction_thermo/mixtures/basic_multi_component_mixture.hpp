// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicMultiComponentMixture
// Description
//   Multi-component mixture.
//   Provides a list of mass fraction fields and helper functions to
//   query mixture composition.
// SourceFiles
//   basic_multi_component_mixture.cpp
#ifndef basic_multi_component_mixture_hpp_
#define basic_multi_component_mixture_hpp_
#include "vol_fields.hpp"
#include "ptr_list.hpp"
#include "species_table.hpp"
#include "type_info.hpp"
namespace mousse
{
class basicMultiComponentMixture
{
protected:
  // Protected data
    //- Table of specie names
    speciesTable species_;
    //- Species mass fractions
    PtrList<volScalarField> Y_;
public:
  //- Run time type information
  TYPE_NAME("basicMultiComponentMixture");
  //- The base class of the mixture
  typedef basicMultiComponentMixture basicMixtureType;
  // Constructors
    //- Construct from dictionary, species names, mesh and phase name
    basicMultiComponentMixture
    (
      const dictionary&,
      const wordList& specieNames,
      const fvMesh&,
      const word&
    );
  //- Destructor
  virtual ~basicMultiComponentMixture()
  {}
  // Member functions
    //- Return the table of species
    inline const speciesTable& species() const;
    //- Return the mass-fraction fields
    inline PtrList<volScalarField>& Y();
    //- Return the const mass-fraction fields
    inline const PtrList<volScalarField>& Y() const;
    //- Return the mass-fraction field for a specie given by index
    inline volScalarField& Y(const label i);
    //- Return the const mass-fraction field for a specie given by index
    inline const volScalarField& Y(const label i) const;
    //- Return the mass-fraction field for a specie given by name
    inline volScalarField& Y(const word& specieName);
    //- Return the const mass-fraction field for a specie given by name
    inline const volScalarField& Y(const word& specieName) const;
    //- Does the mixture include this specie?
    inline bool contains(const word& specieName) const;
};
}  // namespace mousse
#include "basic_multi_component_mixture_i.hpp"
#endif
