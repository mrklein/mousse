// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_part.hpp"
// Global Functions 
template<class Type>
void mousse::ensightPart::writeField
(
  ensightFile& os,
  const Field<Type>& field,
  const bool perNode
) const
{
  if (this->size() && field.size())
  {
    writeHeader(os);
    if (perNode)
    {
      os.writeKeyword("coordinates");
      for
      (
        direction cmpt=0;
        cmpt < pTraits<Type>::nComponents;
        ++cmpt
      )
      {
        writeFieldList(os, field.component(cmpt), labelUList::null());
      }
    }
    else
    {
      FOR_ALL(elementTypes(), elemI)
      {
        const labelUList& idList = elemLists_[elemI];
        if (idList.size())
        {
          os.writeKeyword(elementTypes()[elemI]);
          for
          (
            direction cmpt=0;
            cmpt < pTraits<Type>::nComponents;
            ++cmpt
          )
          {
            writeFieldList(os, field.component(cmpt), idList);
          }
        }
      }
    }
  }
}
