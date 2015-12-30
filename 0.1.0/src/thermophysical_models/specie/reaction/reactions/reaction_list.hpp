// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactionList
// Description
//   List of templated reactions
// SourceFiles
//   reaction_list.cpp
#ifndef reaction_list_hpp_
#define reaction_list_hpp_
#include "ptr_list.hpp"
#include "sl_ptr_list.hpp"
#include "species_table.hpp"
#include "hash_ptr_table.hpp"
#include "reaction.hpp"
#include "file_name.hpp"
namespace mousse
{
template<class ThermoType>
class ReactionList
:
  public SLPtrList<Reaction<ThermoType> >
{
  // Private data
    //- Reference to the table of species
    const speciesTable& species_;
    //- Reference to the thermo database
    const HashPtrTable<ThermoType>& thermoDb_;
    //- The dictionary used for construction
    const dictionary dict_;
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const ReactionList&);
public:
  // Constructors
    //- Construct null
    ReactionList
    (
      const speciesTable& species,
      const HashPtrTable<ThermoType>& thermoDatabase
    );
    //- Construct from dictionary
    ReactionList
    (
      const speciesTable& species,
      const HashPtrTable<ThermoType>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct from file using (Istream)
    ReactionList
    (
      const speciesTable& species,
      const HashPtrTable<ThermoType>& thermoDatabase,
      const fileName& fName
    );
    //- Construct copy
    ReactionList(const ReactionList<ThermoType>& reactions);
  //- Destructor
  ~ReactionList();
  // Public Member Functions
    //- Read reactions from dictionary
    bool readReactionDict();
    //- Write
    void write(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "reaction_list.cpp"
#endif
#endif
