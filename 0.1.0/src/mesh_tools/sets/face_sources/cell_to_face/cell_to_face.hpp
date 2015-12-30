// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellToFace
// Description
//   A topoSetSource to select a faceSet from cells.
//   Either all faces of cell or some other criterion.
//   See implementation.
//   Note: when picking up coupled faces uses cells on neighbouring processors.
// SourceFiles
//   cell_to_face.cpp
#ifndef cell_to_face_hpp_
#define cell_to_face_hpp_
#include "topo_set_source.hpp"
#include "named_enum.hpp"
namespace mousse
{
class cellToFace
:
  public topoSetSource
{
public:
    //- Enumeration defining the valid options
    enum cellAction
    {
      ALL,
      BOTH
    };
private:
    //- Add usage string
    static addToUsageTable usage_;
    static const NamedEnum<cellAction, 2> cellActionNames_;
    //- Name of set to use
    word setName_;
    //- Option
    cellAction option_;
  // Private Member Functions
    //- Depending on face to cell option add to or delete from cellSet.
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TypeName("cellToFace");
  // Constructors
    //- Construct from components
    cellToFace
    (
      const polyMesh& mesh,
      const word& setName,
      const cellAction option
    );
    //- Construct from dictionary
    cellToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    cellToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~cellToFace();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACESETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
