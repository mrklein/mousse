#ifndef LAGRANGIAN_INTERMEDIATE_PHASE_PROPERTIES_PHASE_PROPERTIES_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_PHASE_PROPERTIES_PHASE_PROPERTIES_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phasePropertiesList
// Description
//   Simple container for a list of phase properties

#include "istream.hpp"
#include "vol_fields.hpp"
#include "word_list.hpp"
#include "phase_properties.hpp"


namespace mousse {

class phasePropertiesList
{
  // Private data
    //- List of phase properties
    List<phaseProperties> props_;
    //- List of word representation of phase types
    wordList phaseTypeNames_;
    //- List of state labels
    wordList stateLabels_;
public:
  // Constructors
    //- Construct null
    phasePropertiesList();
    //- Construct from components
    phasePropertiesList
    (
      Istream& is,
      const wordList& gasNames,
      const wordList& liquidNames,
      const wordList& solidNames
    );
  //- Destructor
  ~phasePropertiesList();
  // Public Member Functions
    //- Return the list of phase properties
    const List<phaseProperties>& props() const;
    //- Return the list of word representation of phase types
    const wordList& phaseTypes() const;
    //- Return the list of state labels
    const wordList& stateLabels() const;
    //- Return the size (number of phases)
    label size() const;
  // Member operators
    const phaseProperties& operator[](const label) const;
};

}  // namespace mousse

#endif

