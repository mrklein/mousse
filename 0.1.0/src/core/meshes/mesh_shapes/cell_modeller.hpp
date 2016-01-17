// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellModeller
// Description
//   A static collection of cell models, and a means of looking them up.
// SourceFiles
//   cell_modeller.cpp
//   cell_modeller_io.cpp
//   global_cell_modeller.cpp
#ifndef cell_modeller_hpp_
#define cell_modeller_hpp_
#include "cell_model.hpp"
#include "ptr_list.hpp"
#include "hash_table.hpp"
namespace mousse
{
class cellModeller
{
  // Private data
    //- PtrList of models
    static PtrList<cellModel> models_;
    //- List of model pointers
    static List<cellModel*> modelPtrs_;
    //- HashTable of model pointers
    static HashTable<const cellModel*> modelDictionary_;
public:
  // Constructors
    //- Construct from central "cellModels" file
    cellModeller();
  //- Destructor
  ~cellModeller();
  // Member functions
    //- Look up a model by name and return a pointer to the model or NULL
    static const cellModel* lookup(const word&);
    //- Look up a model by index and return a pointer to the model or NULL
    static const cellModel* lookup(const label i)
    {
      return modelPtrs_[i];
    }
};
}  // namespace mousse
#endif
