// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::foamChemistryReader
// Description
//   Chemistry reader for OpenFOAM format
// SourceFiles
//   foam_chemistry_reader.cpp
#ifndef foam_chemistry_reader_hpp_
#define foam_chemistry_reader_hpp_
#include "chemistry_reader.hpp"
#include "file_name.hpp"
#include "type_info.hpp"
#include "hash_ptr_table.hpp"
#include "label_list.hpp"
#include "species_table.hpp"
#include "atomic_weights.hpp"
namespace mousse
{
template<class ThermoType>
class foamChemistryReader
:
  public chemistryReader<ThermoType>
{
    //- Chemistry/reactions dictionary
    dictionary chemDict_;
    //- Thermo properties dictionary
    dictionary thermoDict_;
    //- Table of the thermodynamic data given in the foamChemistry file
    HashPtrTable<ThermoType> speciesThermo_;
    //- Table of species
    speciesTable& speciesTable_;
    //- List of the reactions
    ReactionList<ThermoType> reactions_;
  // Private Member Functions
    //- Set the species list
    speciesTable& setSpecies(const dictionary& dict, speciesTable& species);
public:
  //- Runtime type information
  TYPE_NAME("foamChemistryReader");
  // Constructors
    //- Construct from foamChemistry and thermodynamics file names
    foamChemistryReader
    (
      const fileName& reactionsFileName,
      speciesTable& species,
      const fileName& thermoFileName
    );
    //- Construct by getting the foamChemistry and thermodynamics file names
    //  from dictionary
    foamChemistryReader
    (
      const dictionary& thermoDict,
      speciesTable& species
    );
    //- Disallow default bitwise copy construct
    foamChemistryReader(const foamChemistryReader&) = delete;
    //- Disallow default bitwise assignment
    foamChemistryReader& operator=(const foamChemistryReader&) = delete;
  //- Destructor
  virtual ~foamChemistryReader()
  {}
  // Member functions
    //- Table of species
    const speciesTable& species() const
    {
      return speciesTable_;
    }
    //- Table of the thermodynamic data given in the foamChemistry file
    const HashPtrTable<ThermoType>& speciesThermo() const
    {
      return speciesThermo_;
    }
    //- List of the reactions
    const ReactionList<ThermoType>& reactions() const
    {
      return reactions_;
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "foam_chemistry_reader.cpp"
#endif
#endif
