// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileControl
// Description
// SourceFiles
//   file_control.cpp
#ifndef file_control_hpp_
#define file_control_hpp_
#include "cell_size_and_alignment_control.hpp"
namespace mousse
{
class fileControl
:
  public cellSizeAndAlignmentControl
{
  // Private data
    const fileName pointsFile_;
    const fileName sizesFile_;
    const fileName alignmentsFile_;
    label maxPriority_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fileControl(const fileControl&);
    //- Disallow default bitwise assignment
    void operator=(const fileControl&);
public:
  //- Runtime type information
  TypeName("fileControl");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    fileControl
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
  //- Destructor
  ~fileControl();
  // Member Functions
    // Access
    // Query
//            //- Return the cell size at the given location
//            virtual scalar cellSize(const point& pt) const;
//
//            //- Return the cell alignment at the given location
//            virtual tensor cellAlignment(const point& pt) const;
//
//            virtual void cellSizeAndAlignment
//            (
//                const point& pt,
//                scalar& size,
//                tensor& alignment
//            ) const;
      virtual label maxPriority() const
      {
        return maxPriority_;
      }
    // Edit
      virtual void cellSizeFunctionVertices
      (
        DynamicList<mousse::point>& pts,
        DynamicList<scalar>& sizes
      ) const;
      virtual void initialVertices
      (
        pointField& pts,
        scalarField& sizes,
        triadField& alignments
      ) const;
};
}  // namespace mousse
#endif
