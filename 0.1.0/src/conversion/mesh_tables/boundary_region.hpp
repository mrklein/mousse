// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boundaryRegion
// Description
//   The boundaryRegion persistent data saved as a Map<dictionary>.
//   The meshReader supports boundaryRegion information.
//   The <tt>constant/boundaryRegion</tt> file is an \c IOMap<dictionary>
//   that is used to save the information persistently.
//   It contains the boundaryRegion information of the following form:
//   \verbatim
//     (
//       INT
//       {
//         BoundaryType    WORD;
//         Label           WORD;
//       }
//       ...
//     )
//   \endverbatim
// SourceFiles
//   boundary_region.cpp
#ifndef boundary_region_hpp_
#define boundary_region_hpp_
#include "poly_mesh.hpp"
#include "map.hpp"
#include "dictionary.hpp"
#include "label_list.hpp"
#include "word_list.hpp"
#include "word_re_list.hpp"
namespace mousse
{
class boundaryRegion
:
  public Map<dictionary>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    boundaryRegion(const boundaryRegion&);
public:
  // Constructors
    //- Construct null
    boundaryRegion();
    //- Construct read from registry, name. instance
    boundaryRegion
    (
      const objectRegistry&,
      const word& name = "boundaryRegion",
      const fileName& instance = "constant"
    );
  //- Destructor
  ~boundaryRegion();
  // Member Functions
    //- Append to the end, return index
    label append(const dictionary&);
    //- Return index corresponding to patch 'name'
    //  returns -1 if not found
    label findIndex(const word& name) const;
    //- Return a Map of (id => name)
    Map<word> names() const;
    //- Return a Map of (id => names) selected by patterns
    Map<word> names(const UList<wordRe>& patterns) const;
    //- Return a Map of (id => type)
    Map<word> boundaryTypes() const;
    //- Return BoundaryType corresponding to patch 'name'
    word boundaryType(const word& name) const;
    //- Read constant/boundaryRegion
    void readDict
    (
      const objectRegistry&,
      const word& name = "boundaryRegion",
      const fileName& instance = "constant"
    );
    //- Write constant/boundaryRegion for later reuse
    void writeDict
    (
      const objectRegistry&,
      const word& name = "boundaryRegion",
      const fileName& instance = "constant"
    ) const;
  // Member Operators
    //- Assignment
    void operator=(const boundaryRegion&);
    //- Assign from Map<dictionary>
    void operator=(const Map<dictionary>&);
  // Friend Functions
    //- Rename regions
    //  each dictionary entry is a single word:
    //  \verbatim
    //      newPatchName    originalName;
    //  \endverbatim
    void rename(const dictionary&);
};
}  // namespace mousse
#endif
