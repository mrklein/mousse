// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceCellSizeFunction
// Description
//   Abstract base class for specifying target cell sizes
// SourceFiles
//   surface_cell_size_function.cpp
#ifndef surface_cell_size_function_hpp_
#define surface_cell_size_function_hpp_
#include "searchable_surface.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class surfaceCellSizeFunction
:
  public dictionary
{
protected:
  // Protected data
    //- Reference to the searchableSurface that surfaceCellSizeFunction
    //  relates to
    const searchableSurface& surface_;
    const dictionary coeffsDict_;
    const scalar& defaultCellSize_;
    //- If cell resizing is allowed, this is the factor of the old cell size
    //  to get the new cell size
    scalar refinementFactor_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    surfaceCellSizeFunction(const surfaceCellSizeFunction&);
    //- Disallow default bitwise assignment
    void operator=(const surfaceCellSizeFunction&);
public:
  //- Runtime type information
  TypeName("surfaceCellSizeFunction");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      surfaceCellSizeFunction,
      dictionary,
      (
        const dictionary& surfaceCellSizeFunctionDict,
        const searchableSurface& surface,
        const scalar& defaultCellSize
      ),
      (surfaceCellSizeFunctionDict, surface, defaultCellSize)
    );
  // Constructors
    //- Construct from components
    surfaceCellSizeFunction
    (
      const word& type,
      const dictionary& surfaceCellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
  // Selectors
    //- Return a reference to the selected surfaceCellSizeFunction
    static autoPtr<surfaceCellSizeFunction> New
    (
      const dictionary& surfaceCellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~surfaceCellSizeFunction();
  // Member Functions
    //- Const access to the details dictionary
    inline const dictionary& coeffsDict() const
    {
      return coeffsDict_;
    }
    virtual scalar interpolate
    (
      const point& pt,
      const label index
    ) const = 0;
};
}  // namespace mousse
#endif
