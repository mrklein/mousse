// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::splitCell
// Description
//   Description of cell after splitting. Contains cellLabel and pointers
//   to cells it it split in. See directedRefinement.
// SourceFiles
//   split_cell.cpp
#ifndef split_cell_hpp_
#define split_cell_hpp_
#include "label.hpp"
namespace mousse
{
// Forward declaration of classes
class splitCell
{
  // Private data
    //- Unsplit cell label. Only uptodate if this cell is 'live'
    //  (i.e. no master or slave)
    label cellI_;
    //- Parent splitCell or null
    splitCell* parent_;
    //- Cells replacing this or null
    splitCell* master_;
    splitCell* slave_;
public:
  // Constructors
    //- Construct from cell number and parent
    splitCell(const label cellI, splitCell* parent);
    //- Disallow default bitwise copy construct
    splitCell(const splitCell&) = delete;
    //- Disallow default bitwise assignment
    splitCell& operator=(const splitCell&) = delete;
  //- Destructor
  ~splitCell();
  // Member Functions
    // Access
      label cellLabel() const
      {
        return cellI_;
      }
      label& cellLabel()
      {
        return cellI_;
      }
      splitCell* parent() const
      {
        return parent_;
      }
      splitCell*& parent()
      {
        return parent_;
      }
      splitCell* master() const
      {
        return master_;
      }
      splitCell*& master()
      {
        return master_;
      }
      splitCell* slave() const
      {
        return slave_;
      }
      splitCell*& slave()
      {
        return slave_;
      }
      //- Check if this is master cell of split
      bool isMaster() const;
      //- Check if this is unrefined (i.e. has no master or slave)
      bool isUnrefined() const;
      //- Returns other half of split cell. I.e. slave if this is master.
      splitCell* getOther() const;
};
}  // namespace mousse
#endif
