// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightPart
// Description
//   Base class for ensightPartCells and ensightPartFaces
// SourceFiles
//   ensight_part.cpp
//   ensight_part_io.cpp
//   ensight_part_templates.cpp
#ifndef ensight_part_hpp_
#define ensight_part_hpp_
#include "ensight_file.hpp"
#include "ensight_geo_file.hpp"
#include "type_info.hpp"
#include "label_list.hpp"
#include "poly_mesh.hpp"
#include "field.hpp"
#include "ioptr_list.hpp"
#include "iostream.hpp"
namespace mousse
{
class ensightPart
{
  // Private data
    // Static data members
    static const List<word> elemTypes_;
protected:
  // Protected data
    //- Part number
    label number_;
    //- Part name (or description)
    string name_;
    //- Simple labelList with a name
    labelListList elemLists_;
    //- Start offset for elemLists_
    label offset_;
    //- Number of elements in this part
    label size_;
    //- Cell or face data
    bool isCellData_;
    //- Material id (numeric)
    label matId_;
    //- pointField referenced
    const pointField& points_;
  // Protected Classes
    //- Track the points used by the part and map global to local indices
    class localPoints
    {
    public:
      //- Number of points used
      label nPoints;
      //- Map global to local indices
      labelList list;
      //- Null constructor
      localPoints()
      :
        nPoints(0),
        list(0)
      {}
      //- Construct for mesh points
      localPoints(const pointField& pts)
      :
        nPoints(0),
        list(pts.size(), -1)
      {}
    };
  // Protected Member Functions
    //- Reconstruct part characteristics (eg, element types) from Istream
    //  A part reconstructed in this manner can be used when writing fields,
    //  but cannot be used to write a new geometry
    void reconstruct(Istream&);
    //- Check for fully defined fields
    bool isFieldDefined(const List<scalar>&) const;
    //- Write the part header
    void writeHeader(ensightFile&, bool withDescription=false) const;
    //- Write a scalar field for idList
    //  A null reference for idList writes the perNode values
    void writeFieldList
    (
      ensightFile& os,
      const List<scalar>& field,
      const labelUList& idList
    ) const;
    //- Track points used
    virtual localPoints calcLocalPoints() const
    {
      return localPoints();
    }
    //- Write connectivities
    virtual void writeConnectivity
    (
      ensightGeoFile&,
      const word& /*key*/,
      const labelUList& /*idList*/,
      const labelUList& /*pointMap*/
    ) const
    {}
public:
  //- Runtime type information
  TYPE_NAME("ensightPart");
  // Constructors
    //- Construct null
    ensightPart();
    //- Construct empty part with number and description
    ensightPart(label partNumber, const string& partDescription);
    //- Construct part with number, description and points reference
    ensightPart
    (
      label partNumber,
      const string& partDescription,
      const pointField& points
    );
    //- Construct as copy
    ensightPart(const ensightPart&);
  // Selectors
    // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      ensightPart,
      istream,
      (
        Istream& is
      ),
      (is)
    );
    //- Construct and return clone
    autoPtr<ensightPart> clone() const
    {
      return autoPtr<ensightPart>(new ensightPart(*this));
    };
    //- Reconstruct part characteristics on freestore from Istream
    //  \sa reconstruct
    static autoPtr<ensightPart> New(Istream&);
  //- Destructor
  virtual ~ensightPart();
  // Static members
    virtual const List<word>& elementTypes() const
    {
      return elemTypes_;
    }
  // Access
    //- Number of elements in this part
    label size() const
    {
      return size_;
    }
    //- Represents cell data
    bool isCellData() const
    {
      return isCellData_;
    }
    //- Represents face data
    bool isFaceData() const
    {
      return !isCellData_;
    }
    //- Part number
    label number() const
    {
      return number_;
    }
    //- Part name or description
    const string& name() const
    {
      return name_;
    }
    //- Material id
    label materialId() const
    {
      return matId_;
    }
    //- non-const access to part name or description
    void name(const string& value)
    {
      name_ = value;
    }
    //- non-const access to material id
    void materialId(const label value)
    {
      matId_ = value;
    }
    //- Simple labelList with a name
    const labelListList& elemLists() const
    {
      return elemLists_;
    }
    //- Offset for element ids
    label offset() const
    {
      return offset_;
    }
  // Edit
    //- Renumber elements
    void renumber(const labelUList&);
    //- Write summary information about the object
    bool writeSummary(Ostream&) const;
    //- Write reconstruction information for the object
    bool writeData(Ostream&) const;
    //- Write geometry
    virtual void writeGeometry(ensightGeoFile&) const
    {}
    //- Helper: write geometry given the pointField
    void writeGeometry(ensightGeoFile&, const pointField&) const;
    //- Write scalar field
    //  optionally write data per node
    void writeScalarField
    (
      ensightFile&,
      const List<scalar>& field,
      const bool perNode = false
    ) const;
    //- Write vector field components
    //  optionally write data per node
    void writeVectorField
    (
      ensightFile&,
      const List<scalar>& field0,
      const List<scalar>& field1,
      const List<scalar>& field2,
      const bool perNode = false
    ) const;
    //- Write generalized field components
    //  optionally write data per node
    template<class Type>
    void writeField
    (
      ensightFile&,
      const Field<Type>&,
      const bool perNode = false
    ) const;
  // Member Operators
    //- Disallow default bitwise assignment
    void operator=(const ensightPart&)
    {
      NOT_IMPLEMENTED("ensightPart::operator=(const ensightPart&)");
    }
  // IOstream Operators
    //- Write data (reconstruction information)
    friend Ostream& operator<<(Ostream&, const ensightPart&);
    //- Write geometry
    friend ensightGeoFile& operator<<(ensightGeoFile&, const ensightPart&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "ensight_part_templates.cpp"
#endif
#endif
