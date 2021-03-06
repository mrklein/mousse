#ifndef FILE_FORMATS_STARCD_CORE_HPP_
#define FILE_FORMATS_STARCD_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STARCDCore
// Description
//   Core routines used when reading/writing pro-STAR vrt/cel/bnd files.

#include "ifstream.hpp"
#include "point_field.hpp"


namespace mousse {
namespace fileFormats {

class STARCDCore
{
protected:
  // Protected Member Functions
    //- Read header
    static bool readHeader(IFstream&, const word& fileSignature);
    //- Write header for fileType (CELL|VERTEX|BOUNDARY)
    static void writeHeader(Ostream&, const word& fileType);
protected:
    enum cellType
    {
      starcdFluidType   = 1,
      starcdSolidType   = 2,
      starcdBaffleType  = 3,
      starcdShellType   = 4,
      starcdLineType    = 5,
      starcdPointType   = 6
    };
    enum shapeType
    {
      starcdPoint = 1,
      starcdLine  = 2,
      starcdShell = 3,
      starcdHex   = 11,
      starcdPrism = 12,
      starcdTet   = 13,
      starcdPyr   = 14,
      starcdPoly  = 255
    };
public:
  // Public Member Functions
    //- Read points from a (.vrt) file
    // The file format is as follows:
    // \verbatim
    // Line 1:
    //   PROSTAR_VERTEX  newline
    //
    // Line 2:
    //   {version} 0 0 0 0 0 0 0  newline
    //
    // Body:
    //   {vertexId}  {x}  {y}  {z}  newline
    // \endverbatim
    static bool readPoints(IFstream&, pointField&, labelList& ids);
    //- Write header and points to (.vrt) file
    static void writePoints(Ostream&, const pointField&);
  // Constructors
    //- Construct null
    STARCDCore();
};

}  // namespace fileFormats
}  // namespace mousse
#endif
