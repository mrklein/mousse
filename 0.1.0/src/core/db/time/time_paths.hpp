// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimePaths
// Description
//   A class for addressing time paths without using the Time class.
// SourceFiles
//   time_paths.cpp
#ifndef time_paths_hpp_
#define time_paths_hpp_
#include "file_name.hpp"
namespace mousse
{
class TimePaths
{
  // Private data
    bool processorCase_;
    const fileName rootPath_;
    fileName globalCaseName_;
    const fileName case_;
    const word system_;
    const word constant_;
public:
  // Constructors
    //- Construct given database name, rootPath and casePath
    TimePaths
    (
      const fileName& rootPath,
      const fileName& caseName,
      const word& systemName,
      const word& constantName
    );
    //- Construct given database name, rootPath and casePath
    TimePaths
    (
      const bool processorCase,
      const fileName& rootPath,
      const fileName& globalCaseName,
      const fileName& caseName,
      const word& systemName,
      const word& constantName
    );
  // Member functions
      //- Return true if this is a processor case
      bool processorCase() const
      {
        return processorCase_;
      }
      //- Return root path
      const fileName& rootPath() const
      {
        return rootPath_;
      }
      //- Return global case name
      const fileName& globalCaseName() const
      {
        return globalCaseName_;
      }
      //- Return case name
      const fileName& caseName() const
      {
        return case_;
      }
      //- Return system name
      const word& system() const
      {
        return system_;
      }
      //- Return system name for the case
      //  which for parallel runs returns ../system()
      fileName caseSystem() const;
      //- Return constant name
      const word& constant() const
      {
        return constant_;
      }
      //- Return constant name for the case
      //  which for parallel runs returns ../constant()
      fileName caseConstant() const;
      //- Return path
      fileName path() const
      {
        return rootPath()/caseName();
      }
      //- Return system path
      fileName systemPath() const
      {
        return path()/system();
      }
      //- Return constant path
      fileName constantPath() const
      {
        return path()/constant();
      }
};
}  // namespace mousse
#endif
