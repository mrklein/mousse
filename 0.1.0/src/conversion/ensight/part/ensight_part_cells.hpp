// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightPartCells
// Description
//   An implementation of ensightPart to hold volume mesh cells.
// SourceFiles
//   ensight_part_cells.cpp
#ifndef ensight_part_cells_hpp_
#define ensight_part_cells_hpp_
#include "ensight_part.hpp"
#include "type_info.hpp"
namespace mousse
{
class ensightPartCells
:
  public ensightPart
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const ensightPartCells&);
    //- Classify the cell types, set elemLists.
    void classify
    (
      const polyMesh&,
      const labelUList& idLabels = labelUList::null()
    );
    //- Track points used
    virtual localPoints calcLocalPoints() const;
    //- Track the points used
    // virtual void makeLocalPointMap();
    //- Element connectivity
    virtual void writeConnectivity
    (
      ensightGeoFile&,
      const word& key,
      const labelUList& idList,
      const labelUList& pointMap
    ) const;
protected:
    //- Addressable ensight element types
    enum elemType
    {
      tetra4Elements,
      pyramid5Elements,
      penta6Elements,
      hexa8Elements,
      nfacedElements
    };
  // Static data members
    static const List<word> elemTypes_;
  // Protected data
    //- Mesh referenced
    const polyMesh& mesh_;
public:
  //- Runtime type information
  TYPE_NAME("ensightCells");
  // Constructors
    //- Construct empty part with number and description
    ensightPartCells(label partNumber, const string& partDescription);
    //- Construct from polyMesh without zones
    ensightPartCells(label partNumber, const polyMesh&);
    //- Construct from polyMesh and list of (non-zoned) cells
    ensightPartCells
    (
      label partNumber,
      const polyMesh&,
      const labelUList&
    );
    //- Construct from polyMesh and cellZone
    ensightPartCells
    (
      label partNumber,
      const polyMesh&,
      const cellZone&
    );
    //- Construct as copy
    ensightPartCells(const ensightPartCells&);
    //- Reconstruct part characteristics (eg, element types) from Istream
    //  A part reconstructed in this manner can be used when writing fields,
    //  but cannot be used to write a new geometry
    //  \sa mousse::ensightPart::reconstruct
    ensightPartCells(Istream&);
    //- Reconstruct part characteristics on freestore from Istream
    static autoPtr<ensightPartCells> New(Istream& is)
    {
      return autoPtr<ensightPartCells>(new ensightPartCells(is));
    }
  //- Destructor
  virtual ~ensightPartCells();
  // Member Functions
    //- Write geometry
    virtual void writeGeometry(ensightGeoFile&) const;
    //- Static listing of the element types
    virtual const List<word>& elementTypes() const
    {
      return elemTypes_;
    }
};
}  // namespace mousse
#endif
