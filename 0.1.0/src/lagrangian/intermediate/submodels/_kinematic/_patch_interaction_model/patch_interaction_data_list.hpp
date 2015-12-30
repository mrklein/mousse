// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchInteractionDataList
// Description
//   List container for patchInteractionData class
#ifndef patch_interaction_data_list_hpp_
#define patch_interaction_data_list_hpp_
#include "patch_interaction_data.hpp"
#include "poly_mesh.hpp"
#include "dictionary.hpp"
#include "label_list.hpp"
namespace mousse
{
class patchInteractionDataList
:
  public List<patchInteractionData>
{
private:
  // Private data
    //- List of patch IDs for each patch group
    labelListList patchGroupIDs_;
public:
  // Constructor
    //- Construct null
    patchInteractionDataList();
    //- Construct copy
    patchInteractionDataList(const patchInteractionDataList& pidl);
    //- Construct from Istream
    patchInteractionDataList(const polyMesh& mesh, const dictionary& dict);
  // Member functions
    //- Return label of group containing patch id
    //  Returns -1 if patch id is not present
    label applyToPatch(const label id) const;
};
}  // namespace mousse
#endif
