// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "profile_model_list.hpp"


// Constructors 
mousse::profileModelList::profileModelList
(
  const dictionary& dict,
  const bool readFields
)
:
  PtrList<profileModel>{},
  dict_{dict}
{
  if (readFields) {
    wordList modelNames{dict.toc()};
    Info << "    Constructing blade profiles:" << endl;
    if (modelNames.size() > 0) {
      this->setSize(modelNames.size());
      FOR_ALL(modelNames, i) {
        const word& modelName = modelNames[i];
        this->set
        (
          i,
          profileModel::New(dict.subDict(modelName))
        );
      }
    } else {
      Info << "        none" << endl;
    }
  }
}


// Destructor 
mousse::profileModelList::~profileModelList()
{}


// Member Functions 
void mousse::profileModelList::connectBlades
(
  const List<word>& names,
  List<label>& addr
) const
{
  // construct the addressing between blade sections and profiles
  FOR_ALL(names, bI) {
    label index = -1;
    const word& profileName = names[bI];
    FOR_ALL(*this, pI) {
      const profileModel& pm = this->operator[](pI);
      if (pm.name() == profileName) {
        index = pI;
        break;
      }
    }
    if (index == -1) {
      List<word> profileNames{size()};
      FOR_ALL(*this, i) {
        const profileModel& pm = this->operator[](i);
        profileNames[i] = pm.name();
      }
      FATAL_ERROR_IN("void mousse::connectBlades(List<word>& names) const")
        << "Profile " << profileName << " could not be found "
        << "in profile list.  Available profiles are"
        << profileNames << exit(FatalError);
    } else {
      addr[bI] = index;
    }
  }
}

