// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_average_item.hpp"


// Static Data Members
namespace mousse {

const word fieldAverageItem::EXT_MEAN = "Mean";
const word fieldAverageItem::EXT_PRIME2MEAN = "Prime2Mean";
template<>
const char* mousse::NamedEnum
<
  mousse::fieldAverageItem::baseType,
  2
>::names[] =
{
  "iteration",
  "time"
};

}

const mousse::NamedEnum<mousse::fieldAverageItem::baseType, 2>
  mousse::fieldAverageItem::baseTypeNames_;


// Constructors 
mousse::fieldAverageItem::fieldAverageItem()
:
  active_{false},
  fieldName_{"unknown"},
  mean_{0},
  meanFieldName_{"unknown"},
  prime2Mean_{0},
  prime2MeanFieldName_{"unknown"},
  base_{ITER},
  window_{-1.0},
  windowName_{""}
{}


mousse::fieldAverageItem::fieldAverageItem(const fieldAverageItem& faItem)
:
  active_{faItem.active_},
  fieldName_{faItem.fieldName_},
  mean_{faItem.mean_},
  meanFieldName_{faItem.meanFieldName_},
  prime2Mean_{faItem.prime2Mean_},
  prime2MeanFieldName_{faItem.prime2MeanFieldName_},
  base_{faItem.base_},
  window_{faItem.window_},
  windowName_{faItem.windowName_}
{}


// Destructor 
mousse::fieldAverageItem::~fieldAverageItem()
{}


// Member Operators 
void mousse::fieldAverageItem::operator=(const fieldAverageItem& rhs)
{
  // Check for assignment to self
  if (this == &rhs) {
    FATAL_ERROR_IN
    (
      "mousse::fieldAverageItem::operator=(const mousse::fieldAverageItem&)"
    )
    << "Attempted assignment to self" << nl
    << abort(FatalError);
  }
  // Set updated values
  active_ = rhs.active_;
  fieldName_ = rhs.fieldName_;
  mean_ = rhs.mean_;
  meanFieldName_ = rhs.meanFieldName_;
  prime2Mean_ = rhs.prime2Mean_;
  prime2MeanFieldName_ = rhs.prime2MeanFieldName_;
  base_ = rhs.base_;
  window_ = rhs.window_;
  windowName_ = rhs.windowName_;
}

