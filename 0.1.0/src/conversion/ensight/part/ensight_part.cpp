// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_part.hpp"
#include "dictionary.hpp"
#include "list_ops.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(ensightPart, 0);
  defineTemplateTypeNameAndDebug(IOPtrList<ensightPart>, 0);
  defineRunTimeSelectionTable(ensightPart, istream);
}
const mousse::List<mousse::word> mousse::ensightPart::elemTypes_(0);
// Protected Member Functions 
bool mousse::ensightPart::isFieldDefined(const List<scalar>& field) const
{
  forAll(elemLists_, elemI)
  {
    const labelUList& idList = elemLists_[elemI];
    forAll(idList, i)
    {
      const label id = idList[i];
      if (id >= field.size() || isnan(field[id]))
      {
        return false;
      }
    }
  }
  return true;
}
// Constructors 
mousse::ensightPart::ensightPart
()
:
  number_(0),
  name_(""),
  elemLists_(0),
  offset_(0),
  size_(0),
  isCellData_(true),
  matId_(0),
  points_(pointField::null())
{}
mousse::ensightPart::ensightPart
(
  label partNumber,
  const string& partDescription
)
:
  number_(partNumber),
  name_(partDescription),
  elemLists_(0),
  offset_(0),
  size_(0),
  isCellData_(true),
  matId_(0),
  points_(pointField::null())
{}
mousse::ensightPart::ensightPart
(
  label partNumber,
  const string& partDescription,
  const pointField& points
)
:
  number_(partNumber),
  name_(partDescription),
  elemLists_(0),
  offset_(0),
  size_(0),
  isCellData_(true),
  matId_(0),
  points_(points)
{}
mousse::ensightPart::ensightPart(const ensightPart& part)
:
  number_(part.number_),
  name_(part.name_),
  elemLists_(part.elemLists_),
  offset_(part.offset_),
  size_(part.size_),
  isCellData_(part.isCellData_),
  matId_(part.matId_),
  points_(part.points_)
{}
// Selectors
mousse::autoPtr<mousse::ensightPart> mousse::ensightPart::New(Istream& is)
{
  const word partType(is);
  istreamConstructorTable::iterator cstrIter =
    istreamConstructorTablePtr_->find(partType);
  if (cstrIter == istreamConstructorTablePtr_->end())
  {
    FatalIOErrorIn
    (
      "ensightPart::New(Istream&)",
      is
    )   << "unknown ensightPart type "
      << partType << nl << nl
      << "Valid ensightPart types are :" << endl
      << istreamConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
  }
  return autoPtr<ensightPart>(cstrIter()(is));
}
// Destructor 
mousse::ensightPart::~ensightPart()
{}
// Member Functions 
void mousse::ensightPart::renumber(const labelUList& origId)
{
  // transform to global values first
  if (offset_)
  {
    forAll(elemLists_, elemI)
    {
      labelList& idList = elemLists_[elemI];
      forAll(idList, i)
      {
        idList[i] += offset_;
      }
    }
    offset_ = 0;
  }
  if (origId.size())
  {
    forAll(elemLists_, elemI)
    {
      inplaceRenumber(origId, elemLists_[elemI]);
    }
  }
}
