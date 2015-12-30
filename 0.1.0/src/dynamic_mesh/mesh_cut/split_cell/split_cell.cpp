// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "split_cell.hpp"
#include "error.hpp"
// Constructors 
// Construct from cell number and parent
mousse::splitCell::splitCell(const label cellI, splitCell* parent)
:
  cellI_(cellI),
  parent_(parent),
  master_(NULL),
  slave_(NULL)
{}
// Destructor 
mousse::splitCell::~splitCell()
{
  splitCell* myParent = parent();
  if (myParent)
  {
    // Make sure parent does not refer to me anymore.
    if (myParent->master() == this)
    {
      myParent->master() = NULL;
    }
    else if (myParent->slave() == this)
    {
      myParent->slave() = NULL;
    }
    else
    {
      FatalErrorIn("splitCell::~splitCell()") << "this not equal to"
        << " parent's master or slave pointer" << endl
        << "Cell:" << cellLabel() << abort(FatalError);
    }
  }
}
// Member Functions 
bool mousse::splitCell::isMaster() const
{
  splitCell* myParent = parent();
  if (!myParent)
  {
    FatalErrorIn("splitCell::isMaster()") << "parent not set"
      << "Cell:" << cellLabel() << abort(FatalError);
    return false;
  }
  else if (myParent->master() == this)
  {
    return true;
  }
  else if (myParent->slave() == this)
  {
    return false;
  }
  else
  {
    FatalErrorIn("splitCell::isMaster()") << "this not equal to"
      << " parent's master or slave pointer" << endl
      << "Cell:" << cellLabel() << abort(FatalError);
    return false;
  }
}
bool mousse::splitCell::isUnrefined() const
{
  return !master() && !slave();
}
mousse::splitCell* mousse::splitCell::getOther() const
{
  splitCell* myParent = parent();
  if (!myParent)
  {
    FatalErrorIn("splitCell::getOther()") << "parent not set"
      << "Cell:" << cellLabel() << abort(FatalError);
    return NULL;
  }
  else if (myParent->master() == this)
  {
    return myParent->slave();
  }
  else if (myParent->slave() == this)
  {
    return myParent->master();
  }
  else
  {
    FatalErrorIn("splitCell::getOther()") << "this not equal to"
      << " parent's master or slave pointer" << endl
      << "Cell:" << cellLabel() << abort(FatalError);
    return NULL;
  }
}
