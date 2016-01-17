// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vtkUnstructuredReader
// Description
//   Reader for vtk unstructured_grid legacy files. Supports single CELLS, POINTS
//   etc. entry only.
//   - all integer types (int, unsigned_int, long etc.) become mousse::label
//   - all real types (float, double) become mousse::scalar
//   - POINTS becomes OpenFOAM points
//   - CELLS gets split into OpenFOAM
//     - cells
//     - faces
//     - lines
//   - CELL_DATA or POINT_DATA gets stored on the corresponding objectRegistry
//    in original vtk numbering order so use e.g. faceMap() to go from entry
//    in faces() back to vtk numbering.
// SourceFiles
//   vtk_unstructured_reader.cpp

#ifndef vtk_unstructured_reader_hpp_
#define vtk_unstructured_reader_hpp_

#include "object_registry.hpp"
#include "cell_shape_list.hpp"
#include "hash_set.hpp"
#include "named_enum.hpp"

namespace mousse
{

class vtkUnstructuredReader
{

public:

  // Public data types

    //- Enumeration defining the vtk data types
    enum vtkDataType
    {
      VTK_INT,
      VTK_UINT,
      VTK_LONG,
      VTK_ULONG,
      VTK_FLOAT,
      VTK_DOUBLE,
      VTK_STRING,
      VTK_ID
    };
    static const NamedEnum<vtkDataType, 8> vtkDataTypeNames;

    //- Enumeration defining the vtk dataset types
    enum vtkDataSetType
    {
      VTK_FIELD,
      VTK_SCALARS,
      VTK_VECTORS
    };
    static const NamedEnum<vtkDataSetType, 3> vtkDataSetTypeNames;

    //- Enumeration defining the parse mode - what type of data is being
    //  read
    enum parseMode
    {
      NOMODE,
      UNSTRUCTURED_GRID,
      POLYDATA,
      CELL_DATA,
      POINT_DATA
    };
    static const NamedEnum<parseMode, 5> parseModeNames;

    //- Enumeration defining the cell types
    enum vtkTypes
    {
      VTK_EMPTY_CELL       = 0,
      VTK_VERTEX           = 1,
      VTK_POLY_VERTEX      = 2,
      VTK_LINE             = 3,
      VTK_POLY_LINE        = 4,
      VTK_TRIANGLE         = 5,
      VTK_TRIANGLE_STRIP   = 6,
      VTK_POLYGON          = 7,
      VTK_PIXEL            = 8,
      VTK_QUAD             = 9,
      VTK_TETRA            = 10,
      VTK_VOXEL            = 11,
      VTK_HEXAHEDRON       = 12,
      VTK_WEDGE            = 13,
      VTK_PYRAMID          = 14,
      VTK_PENTAGONAL_PRISM = 15,
      VTK_HEXAGONAL_PRISM  = 16,
    };

private:

  //- Header
  string header_;

  //- Title
  string title_;

  //- DataType
  string dataType_;

  // Geometry

    //- Points
    pointField points_;

    //- 3D cells.
    cellShapeList cells_;

    //- Map from cells back to original ID
    labelList cellMap_;

    //- 2D cells (=faces)
    faceList faces_;

    //- Map from faces back to original ID
    labelList faceMap_;

    //- 1D cells (=edges)
    labelListList lines_;
    labelList lineMap_;

  // Data

    //- Cell based fields
    objectRegistry cellData_;

    //- Point based fields
    objectRegistry pointData_;

    //- Other fields
    objectRegistry otherData_;

  // Private Member Functions

    template<class T>
    void readBlock
    (
      Istream& inFile,
      const label n,
      List<T>& lst
    ) const;

    void warnUnhandledType
    (
      Istream& inFile,
      const label type,
      labelHashSet& warningGiven
    ) const;

    void extractCells
    (
      Istream& inFile,
      const labelList& cellTypes,
      const labelList& cellVertData
    );

    void readField
    (
      ISstream& inFile,
      objectRegistry& obj,
      const word& arrayName,
      const word& dataType,
      const label size
    ) const;

    wordList readFieldArray
    (
      ISstream& inFile,
      objectRegistry& obj,
      const label wantedSize
    ) const;

    objectRegistry& selectRegistry(const parseMode readMode);

    void read(ISstream& inFile);

public:

  //- Runtime type information
  CLASS_NAME("vtkUnstructuredReader");

  // Constructors

    //- Construct from Istream, read all
    vtkUnstructuredReader(const objectRegistry& obr, ISstream&);

    //- Dissallow assignment
    void operator=(const vtkUnstructuredReader&) = delete;

  // Member Functions

    //- Header
    const string header() const
    {
      return header_;
    }

    //- Title
    const string& title() const
    {
      return title_;
    }

    //- DataType
    const string& dataType() const
    {
      return dataType_;
    }

    //- Points
    const pointField& points() const
    {
      return points_;
    }

    pointField& points()
    {
      return points_;
    }

    //- 3D cells.
    const cellShapeList& cells() const
    {
      return cells_;
    }

    cellShapeList& cells()
    {
      return cells_;
    }

    const labelList& cellMap() const
    {
      return cellMap_;
    }

    //- 2D cells (=faces)
    const faceList& faces() const
    {
      return faces_;
    }

    faceList& faces()
    {
      return faces_;
    }

    const labelList& faceMap() const
    {
      return faceMap_;
    }

    //- 1D cells (=open lines)
    const labelListList& lines() const
    {
      return lines_;
    }

    labelListList& lines()
    {
      return lines_;
    }

    const labelList& lineMap() const
    {
      return lineMap_;
    }

    //- Cell based fields
    const objectRegistry& cellData() const
    {
      return cellData_;
    }

    objectRegistry& cellData()
    {
      return cellData_;
    }

    //- Point based fields
    const objectRegistry& pointData() const
    {
      return pointData_;
    }

    objectRegistry& pointData()
    {
      return pointData_;
    }

    //- Other fields
    const objectRegistry& otherData() const
    {
      return otherData_;
    }

    objectRegistry& otherData()
    {
      return otherData_;
    }

    //- Debug: print contents of objectRegistry
    template<class Type>
    void printFieldStats(const objectRegistry&) const;

};

}  // namespace mousse

#ifdef NoRepository
#   include "vtk_unstructured_reader_templates.cpp"
#endif
#endif
