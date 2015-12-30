// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceToCell
// Description
//   A topoSetSource to select cells based on usage in faces.
// SourceFiles
//   face_to_cell.cpp
#ifndef face_to_cell_hpp_
#define face_to_cell_hpp_
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class faceToCell
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum faceAction
    {
      NEIGHBOUR,
      OWNER,
      ANY,
      ALL
    };
private:
    static const NamedEnum<faceAction, 4> faceActionNames_;
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of set to use
    word setName_;
    //- Option
    faceAction option_;
  // Private Member Functions
    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("faceToCell");
  // Constructors
    //- Construct from components
    faceToCell
    (
      const polyMesh& mesh,
      const word& setName,
      const faceAction option
    );
    //- Construct from dictionary
    faceToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    faceToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~faceToCell();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
