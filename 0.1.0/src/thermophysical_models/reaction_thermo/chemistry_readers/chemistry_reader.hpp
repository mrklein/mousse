// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemistryReader
// Description
//   Abstract class for reading chemistry
// SourceFiles
//   chemistry_reader.cpp
#ifndef chemistry_reader_hpp_
#define chemistry_reader_hpp_
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "reaction.hpp"
#include "reaction_list.hpp"
namespace mousse
{
template<class ThermoType>
class chemistryReader
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    chemistryReader(const chemistryReader&);
    //- Disallow default bitwise assignment
    void operator=(const chemistryReader&);
public:
  //- Runtime type information
  TypeName("chemistryReader");
  //- The type of thermo package the reader was instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct null
    chemistryReader()
    {}
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
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
#ifdef NoRepository
#   include "chemistry_reader.cpp"
#endif
#define makeChemistryReader(Thermo)                                           \
  defineTemplateTypeNameAndDebug(chemistryReader<Thermo>, 0);               \
  defineTemplateRunTimeSelectionTable(chemistryReader<Thermo>, dictionary)
#define makeChemistryReaderType(Reader, Thermo)                               \
  defineNamedTemplateTypeNameAndDebug(Reader<Thermo>, 0);                   \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader<Thermo> > \
    add##Reader##Thermo##ConstructorToTable_
// for non-templated chemistry readers
#define addChemistryReaderType(Reader, Thermo)                                \
  defineTypeNameAndDebug(Reader, 0);                                        \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>          \
    add##Reader##Thermo##ConstructorToTable_
// for templated chemistry readers
#define addTemplateChemistryReaderType(Reader, Thermo)                        \
  defineNamedTemplateTypeNameAndDebug(Reader, 0);                           \
  chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>          \
    add##Reader##Thermo##ConstructorToTable_
#endif
