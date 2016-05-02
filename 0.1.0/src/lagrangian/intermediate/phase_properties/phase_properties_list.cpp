// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_properties_list.hpp"


// Constructors 
mousse::phasePropertiesList::phasePropertiesList()
:
  props_{},
  phaseTypeNames_{},
  stateLabels_{}
{}


mousse::phasePropertiesList::phasePropertiesList
(
  Istream& is,
  const wordList& gasNames,
  const wordList& liquidNames,
  const wordList& solidNames
)
:
  props_{is},
  phaseTypeNames_{},
  stateLabels_{}
{
  FOR_ALL(props_, i) {
    props_[i].reorder(gasNames, liquidNames, solidNames);
  }
  phaseTypeNames_.setSize(props_.size());
  stateLabels_.setSize(props_.size());
  FOR_ALL(props_, i) {
    phaseTypeNames_[i] = props_[i].phaseTypeName();
    stateLabels_[i] = props_[i].stateLabel();
  }
}


// Destructor 
mousse::phasePropertiesList::~phasePropertiesList()
{}


// Member Functions 
const mousse::List<mousse::phaseProperties>&
mousse::phasePropertiesList::props() const
{
  return props_;
}


const mousse::wordList& mousse::phasePropertiesList::phaseTypes() const
{
  return phaseTypeNames_;
}


const mousse::wordList& mousse::phasePropertiesList::stateLabels() const
{
  return stateLabels_;
}


mousse::label mousse::phasePropertiesList::size() const
{
  return props_.size();
}


const mousse::phaseProperties&
mousse::phasePropertiesList::operator[](const label phaseI) const
{
  return props_[phaseI];
}

