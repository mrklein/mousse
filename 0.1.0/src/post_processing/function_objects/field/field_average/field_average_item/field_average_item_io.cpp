// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_average_item.hpp"
#include "iostreams.hpp"
#include "dictionary_entry.hpp"
// Constructors 
mousse::fieldAverageItem::fieldAverageItem(Istream& is)
:
  active_(false),
  fieldName_("unknown"),
  mean_(0),
  meanFieldName_("unknown"),
  prime2Mean_(0),
  prime2MeanFieldName_("unknown"),
  base_(ITER),
  window_(-1.0)
{
  is.check("mousse::fieldAverageItem::fieldAverageItem(mousse::Istream&)");
  const dictionaryEntry entry(dictionary::null, is);
  fieldName_ = entry.keyword();
  entry.lookup("mean") >> mean_;
  entry.lookup("prime2Mean") >> prime2Mean_;
  base_ = baseTypeNames_[entry.lookup("base")];
  window_ = entry.lookupOrDefault<scalar>("window", -1.0);
  windowName_ = entry.lookupOrDefault<word>("windowName", "");
  meanFieldName_ = fieldName_ + EXT_MEAN;
  prime2MeanFieldName_ = fieldName_ + EXT_PRIME2MEAN;
  if ((window_ > 0) && (windowName_ != ""))
  {
    meanFieldName_ = meanFieldName_ + "_" + windowName_;
    prime2MeanFieldName_ = prime2MeanFieldName_ + "_" + windowName_;
  }
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, fieldAverageItem& faItem)
{
  is.check
  (
    "mousse::Istream& mousse::operator>>"
    "(mousse::Istream&, mousse::fieldAverageItem&)"
  );
  const dictionaryEntry entry(dictionary::null, is);
  faItem.active_ = false;
  faItem.fieldName_ = entry.keyword();
  entry.lookup("mean") >> faItem.mean_;
  entry.lookup("prime2Mean") >> faItem.prime2Mean_;
  faItem.base_ = faItem.baseTypeNames_[entry.lookup("base")];
  faItem.window_ = entry.lookupOrDefault<scalar>("window", -1.0);
  faItem.windowName_ = entry.lookupOrDefault<word>("windowName", "");
  faItem.meanFieldName_ = faItem.fieldName_ + fieldAverageItem::EXT_MEAN;
  faItem.prime2MeanFieldName_ =
    faItem.fieldName_ + fieldAverageItem::EXT_PRIME2MEAN;
  if ((faItem.window_ > 0) && (faItem.windowName_ != ""))
  {
    faItem.meanFieldName_ =
      faItem.meanFieldName_ + "_" + faItem.windowName_;
    faItem.prime2MeanFieldName_ =
      faItem.prime2MeanFieldName_ + "_" + faItem.windowName_;
  }
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const fieldAverageItem& faItem)
{
  os.check
  (
    "mousse::Ostream& mousse::operator<<"
    "(mousse::Ostream&, const mousse::fieldAverageItem&)"
  );
  os  << faItem.fieldName_ << nl << token::BEGIN_BLOCK << nl;
  os.writeKeyword("mean") << faItem.mean_ << token::END_STATEMENT << nl;
  os.writeKeyword("prime2Mean") << faItem.mean_
    << token::END_STATEMENT << nl;
  os.writeKeyword("base") << faItem.baseTypeNames_[faItem.base_]
    << token::END_STATEMENT << nl;
  if (faItem.window_ > 0)
  {
    os.writeKeyword("window") << faItem.window_
      << token::END_STATEMENT << nl;
    if (faItem.windowName_ != "")
    {
      os.writeKeyword("windowName") << faItem.windowName_
        << token::END_STATEMENT << nl;
    }
  }
  os  << token::END_BLOCK << nl;
  os.check
  (
    "mousse::Ostream& mousse::operator<<"
    "(mousse::Ostream&, const mousse::fieldAverageItem&)"
  );
  return os;
}
