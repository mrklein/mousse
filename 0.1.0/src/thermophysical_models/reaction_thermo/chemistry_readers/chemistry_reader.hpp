#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_CHEMISTRY_READERS_CHEMISTRY_READER_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_CHEMISTRY_READERS_CHEMISTRY_READER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemistryReader
// Description
//   Abstract class for reading chemistry

#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "reaction.hpp"
#include "reaction_list.hpp"


namespace mousse {

template<class ThermoType>
class chemistryReader
{
public:
  //- Runtime type information
  TYPE_NAME("chemistryReader");
  //- The type of thermo package the reader was instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct null
    chemistryReader()
    {}
    //- Disallow default bitwise copy construct
    chemistryReader(const chemistryReader&) = delete;
    //- Disallow default bitwise assignment
    chemistryReader& operator=(const chemistryReader&) = delete;
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      chemistryReader,
      dictionary,
      (
        const dictionary& thermoDict,
        speciesTable& species
      ),
      (thermoDict, species)
    );
  // Selectors
    //- Select constructed from dictionary
    static autoPtr<chemistryReader> New
    (
      const dictionary& thermoDict,
      speciesTable& species
    );
  //- Destructor
  virtual ~chemistryReader()
  {}
  // Member Functions
    //- Return access to the list of species
    virtual const speciesTable& species() const = 0;
    //- Return access to the thermo packages
    virtual const HashPtrTable<ThermoType>& speciesThermo() const = 0;
    //- Return access to the list of reactions
    virtual const ReactionList<ThermoType>& reactions() const = 0;
};

}  // namespace mousse

#include "chemistry_reader.ipp"

#define MAKE_CHEMISTRY_READER(Thermo)                                         \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(chemistryReader<Thermo>, 0);            \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(chemistryReader<Thermo>, dictionary)

#define MAKE_CHEMISTRY_READER_TYPE(Reader, Thermo)                            \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(Reader<Thermo>, 0);               \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader<Thermo> >   \
    add##Reader##Thermo##ConstructorToTable_

// for non-templated chemistry readers
#define ADD_CHEMISTRY_READER_TYPE(Reader, Thermo)                             \
  DEFINE_TYPE_NAME_AND_DEBUG(Reader, 0);                                      \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>            \
    add##Reader##Thermo##ConstructorToTable_

// for templated chemistry readers
#define ADD_TEMPLATE_CHEMISTRY_READER_TYPE(Reader, Thermo)                    \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(Reader, 0);                       \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>            \
    add##Reader##Thermo##ConstructorToTable_

#endif
