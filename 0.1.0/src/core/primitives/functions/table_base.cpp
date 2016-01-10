// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "table_base.hpp"
#include "time.hpp"
#include "interpolation_weights.hpp"

// Private Member Functions 
template<class Type>
const mousse::interpolationWeights& mousse::TableBase<Type>::interpolator() const
{
  if (interpolatorPtr_.empty())
  {
    // Re-work table into linear list
    tableSamplesPtr_.reset(new scalarField{table_.size()});
    scalarField& tableSamples = tableSamplesPtr_();
    FOR_ALL(table_, i)
    {
      tableSamples[i] = table_[i].first();
    }
    interpolatorPtr_ = interpolationWeights::New
    (
      interpolationScheme_,
      tableSamples
    );
  }

  return interpolatorPtr_();
}


// Constructors 
template<class Type>
mousse::TableBase<Type>::TableBase(const word& name, const dictionary& dict)
:
  DataEntry<Type>{name},
  name_{name},
  boundsHandling_
  {
    wordToBoundsHandling
    (
      dict.lookupOrDefault<word>("outOfBounds", "clamp")
    )
  },
  interpolationScheme_
  {
    dict.lookupOrDefault<word>("interpolationScheme", "linear")
  },
  table_{},
  dimensions_{dimless}
{}


template<class Type>
mousse::TableBase<Type>::TableBase(const TableBase<Type>& tbl)
:
  DataEntry<Type>{tbl},
  name_{tbl.name_},
  boundsHandling_{tbl.boundsHandling_},
  interpolationScheme_{tbl.interpolationScheme_},
  table_{tbl.table_},
  dimensions_{tbl.dimensions_},
  tableSamplesPtr_{tbl.tableSamplesPtr_},
  interpolatorPtr_{tbl.interpolatorPtr_}
{}


// Destructor 
template<class Type>
mousse::TableBase<Type>::~TableBase()
{}


// Member Functions 
template<class Type>
mousse::word mousse::TableBase<Type>::boundsHandlingToWord
(
  const boundsHandling& bound
) const
{
  word enumName{"warn"};
  switch (bound)
  {
    case ERROR:
    {
      enumName = "error";
      break;
    }
    case WARN:
    {
      enumName = "warn";
      break;
    }
    case CLAMP:
    {
      enumName = "clamp";
      break;
    }
    case REPEAT:
    {
      enumName = "repeat";
      break;
    }
  }
  return enumName;
}


template<class Type>
typename mousse::TableBase<Type>::boundsHandling
mousse::TableBase<Type>::wordToBoundsHandling
(
  const word& bound
) const
{
  if (bound == "error")
  {
    return ERROR;
  }
  else if (bound == "warn")
  {
    return WARN;
  }
  else if (bound == "clamp")
  {
    return CLAMP;
  }
  else if (bound == "repeat")
  {
    return REPEAT;
  }
  else
  {
    WARNING_IN("mousse::TableBase<Type>::wordToBoundsHandling(const word&)")
      << "bad outOfBounds specifier " << bound << " using 'warn'"
      << endl;
    return WARN;
  }
}


template<class Type>
typename mousse::TableBase<Type>::boundsHandling
mousse::TableBase<Type>::outOfBounds
(
  const boundsHandling& bound
)
{
  boundsHandling prev = boundsHandling_;
  boundsHandling_ = bound;
  return prev;
}


template<class Type>
void mousse::TableBase<Type>::check() const
{
  if (!table_.size())
  {
    FATAL_ERROR_IN("mousse::TableBase<Type>::check() const")
      << "Table for entry " << this->name_ << " is invalid (empty)"
      << nl << exit(FatalError);
  }
  label n = table_.size();
  scalar prevValue = table_[0].first();
  for (label i = 1; i < n; ++i)
  {
    const scalar currValue = table_[i].first();
    // avoid duplicate values (divide-by-zero error)
    if (currValue <= prevValue)
    {
      FATAL_ERROR_IN("mousse::TableBase<Type>::check() const")
        << "out-of-order value: " << currValue << " at index " << i
        << exit(FatalError);
    }
    prevValue = currValue;
  }
}


template<class Type>
bool mousse::TableBase<Type>::checkMinBounds
(
  const scalar x,
  scalar& xDash
) const
{
  if (x < table_[0].first())
  {
    switch (boundsHandling_)
    {
      case ERROR:
      {
        FATAL_ERROR_IN
        (
          "bool mousse::TableBase<Type>::checkMinBounds"
          "("
            "const scalar, "
            "scalar&"
          ") const"
        )
        << "value (" << x << ") underflow"
        << exit(FatalError);
        break;
      }
      case WARN:
      {
        WARNING_IN
        (
          "bool mousse::TableBase<Type>::checkMinBounds"
          "("
            "const scalar, "
            "scalar&"
          ") const"
        )
        << "value (" << x << ") underflow" << nl
        << endl;
        // fall-through to 'CLAMP'
      }
      case CLAMP:
      {
        xDash = table_[0].first();
        return true;
        break;
      }
      case REPEAT:
      {
        // adjust x to >= minX
        scalar span = table_.last().first() - table_[0].first();
        xDash = fmod(x - table_[0].first(), span) + table_[0].first();
        break;
      }
    }
  }
  else
  {
    xDash = x;
  }
  return false;
}


template<class Type>
bool mousse::TableBase<Type>::checkMaxBounds
(
  const scalar x,
  scalar& xDash
) const
{
  if (x > table_.last().first())
  {
    switch (boundsHandling_)
    {
      case ERROR:
      {
        FATAL_ERROR_IN
        (
          "bool mousse::TableBase<Type>::checkMaxBounds"
          "("
            "const scalar, "
            "scalar&"
          ") const"
        )
        << "value (" << x << ") overflow"
        << exit(FatalError);
        break;
      }
      case WARN:
      {
        WARNING_IN
        (
          "bool mousse::TableBase<Type>::checkMaxBounds"
          "("
            "const scalar, "
            "scalar&"
          ") const"
        )
        << "value (" << x << ") overflow" << nl
        << endl;
        // fall-through to 'CLAMP'
      }
      case CLAMP:
      {
        xDash = table_.last().first();
        return true;
        break;
      }
      case REPEAT:
      {
        // adjust x to >= minX
        scalar span = table_.last().first() - table_[0].first();
        xDash = fmod(x - table_[0].first(), span) + table_[0].first();
        break;
      }
    }
  }
  else
  {
    xDash = x;
  }

  return false;
}


template<class Type>
void mousse::TableBase<Type>::convertTimeBase(const Time& t)
{
  FOR_ALL(table_, i)
  {
    scalar value = table_[i].first();
    table_[i].first() = t.userTimeToTime(value);
  }
  tableSamplesPtr_.clear();
  interpolatorPtr_.clear();
}


template<class Type>
Type mousse::TableBase<Type>::value(const scalar x) const
{
  scalar xDash = x;
  if (checkMinBounds(x, xDash))
  {
    return table_[0].second();
  }
  if (checkMaxBounds(xDash, xDash))
  {
    return table_.last().second();
  }
  // Use interpolator
  interpolator().valueWeights(xDash, currentIndices_, currentWeights_);
  Type t = currentWeights_[0]*table_[currentIndices_[0]].second();
  for (label i = 1; i < currentIndices_.size(); i++)
  {
    t += currentWeights_[i]*table_[currentIndices_[i]].second();
  }
  return t;
}


template<class Type>
Type mousse::TableBase<Type>::integrate(const scalar x1, const scalar x2) const
{
  // Use interpolator
  interpolator().integrationWeights(x1, x2, currentIndices_, currentWeights_);
  Type sum = currentWeights_[0]*table_[currentIndices_[0]].second();
  for (label i = 1; i < currentIndices_.size(); i++)
  {
   sum += currentWeights_[i]*table_[currentIndices_[i]].second();
  }
  return sum;
}


template<class Type>
mousse::dimensioned<Type> mousse::TableBase<Type>::
dimValue(const scalar x) const
{
  return dimensioned<Type>{"dimensionedValue", dimensions_, this->value(x)};
}


template<class Type>
mousse::dimensioned<Type> mousse::TableBase<Type>::dimIntegrate
(
  const scalar x1, const scalar x2
) const
{
  return dimensioned<Type>
  {
    "dimensionedValue",
    dimensions_,
    this->integrate(x2, x1)
  };
}


template<class Type>
mousse::tmp<mousse::scalarField> mousse::TableBase<Type>::x() const
{
  tmp<scalarField> tfld{new scalarField{table_.size(), 0.0}};
  scalarField& fld = tfld();
  FOR_ALL(table_, i)
  {
    fld[i] = table_[i].first();
  }
  return tfld;
}


template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::TableBase<Type>::y() const
{
  tmp<Field<Type>> tfld{new Field<Type>{table_.size(), pTraits<Type>::zero}};
  Field<Type>& fld = tfld();
  FOR_ALL(table_, i)
  {
    fld[i] = table_[i].second();
  }

  return tfld;
}

//  IOStream operators
#include "table_base_io.cpp"
