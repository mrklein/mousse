// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "ioobject_list.hpp"
#include "ioptr_list.hpp"
#include "vol_fields.hpp"
#include "string_list_ops.hpp"
#include "time_selector.hpp"


using namespace mousse;


namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(IOPtrList<entry>, 0);

}


// Extract groupPatch (= shortcut) info from boundary file info
HashTable<wordList, word> extractPatchGroups(const dictionary& boundaryDict)
{
  HashTable<wordList, word> groupToPatch;
  FOR_ALL_CONST_ITER(dictionary, boundaryDict, iter) {
    const word& patchName = iter().keyword();
    const dictionary& patchDict = iter().dict();
    wordList groups;
    if (!patchDict.readIfPresent("inGroups", groups))
      continue;
    FOR_ALL(groups, i) {
      auto fndGroup = groupToPatch.find(groups[i]);
      if (fndGroup == groupToPatch.end()) {
        groupToPatch.insert(groups[i], wordList{1, patchName});
      } else {
        fndGroup().append(patchName);
      }
    }
  }
  return groupToPatch;
}


bool merge
(
  dictionary&,
  const dictionary&,
  const bool,
  const HashTable<wordList, word>&
);


// Add thisEntry to dictionary thisDict.
bool addEntry
(
  dictionary& thisDict,
  entry& thisEntry,
  const entry& mergeEntry,
  const bool literalRE,
  const HashTable<wordList, word>& shortcuts
)
{
  bool changed = false;
  // Recursively merge sub-dictionaries
  // TODO: merge without copying
  if (thisEntry.isDict() && mergeEntry.isDict()) {
    if (merge(const_cast<dictionary&>(thisEntry.dict()),
              mergeEntry.dict(),
              literalRE,
              shortcuts)) {
      changed = true;
    }
  } else {
    // Should use in-place modification instead of adding
    thisDict.add(mergeEntry.clone(thisDict).ptr(), true);
    changed = true;
  }
  return changed;
}


// List of indices into thisKeys
labelList findMatches
(
  const HashTable<wordList, word>& shortcuts,
  const wordList& shortcutNames,
  const wordList& thisKeys,
  const keyType& key
)
{
  labelList matches;
  if (key.isPattern()) {
    // Wildcard match
    matches = findStrings(key, thisKeys);
  } else if (shortcuts.size()) {
    // See if patchGroups expand to valid thisKeys
    labelList indices = findStrings(key, shortcutNames);
    FOR_ALL(indices, i) {
      const word& name = shortcutNames[indices[i]];
      const wordList& keys = shortcuts[name];
      FOR_ALL(keys, j) {
        label index = findIndex(thisKeys, keys[j]);
        if (index != -1) {
          matches.append(index);
        }
      }
    }
  }
  return matches;
}


// Dictionary merging/editing.
// literalRE:
// - true: behave like dictionary::merge, i.e. add regexps just like
//   any other key.
// - false : interpret wildcard as a rule for items to be matched.
bool merge
(
  dictionary& thisDict,
  const dictionary& mergeDict,
  const bool literalRE,
  const HashTable<wordList, word>& shortcuts
)
{
  const wordList shortcutNames{shortcuts.toc()};
  bool changed = false;
  // Save current (non-wildcard) keys before adding items.
  HashSet<word> thisKeysSet;

  {
    List<keyType> keys = thisDict.keys(false);
    FOR_ALL(keys, i) {
      thisKeysSet.insert(keys[i]);
    }
  }

  // Pass 1. All literal matches
  FOR_ALL_CONST_ITER(IDLList<entry>, mergeDict, mergeIter) {
    const keyType& key = mergeIter().keyword();
    if (key[0] == '~') {
      word eraseKey = key(1, key.size()-1);
      if (thisDict.remove(eraseKey)) {
        // Mark thisDict entry as having been match for wildcard
        // handling later on.
        thisKeysSet.erase(eraseKey);
      }
      changed = true;
    } else if (literalRE || !(key.isPattern() || shortcuts.found(key))) {
      entry* entryPtr = thisDict.lookupEntryPtr(key, false, false);
      if (entryPtr) {
        // Mark thisDict entry as having been match for wildcard
        // handling later on.
        thisKeysSet.erase(entryPtr->keyword());
        if (addEntry
            (
              thisDict,
              *entryPtr,
              mergeIter(),
              literalRE,
              shortcuts
            )) {
          changed = true;
        }
      } else {
        // not found - just add
        thisDict.add(mergeIter().clone(thisDict).ptr());
        changed = true;
      }
    }
  }
  // Pass 2. Wildcard or shortcut matches (if any) on any non-match keys.
  if (!literalRE && thisKeysSet.size() > 0) {
    // Pick up remaining dictionary entries
    wordList thisKeys{thisKeysSet.toc()};
    FOR_ALL_CONST_ITER(IDLList<entry>, mergeDict, mergeIter) {
      const keyType& key = mergeIter().keyword();
      if (key[0] == '~') {
        word eraseKey = key(1, key.size()-1);
        // List of indices into thisKeys
        labelList matches
        {
          findMatches
          (
            shortcuts,
            shortcutNames,
            thisKeys,
            eraseKey
          )
        };
        // Remove all matches
        FOR_ALL(matches, i) {
          const word& thisKey = thisKeys[matches[i]];
          thisKeysSet.erase(thisKey);
        }
        changed = true;
      } else {
        // List of indices into thisKeys
        labelList matches
        {
          findMatches
          (
            shortcuts,
            shortcutNames,
            thisKeys,
            key
          )
        };
        // Add all matches
        FOR_ALL(matches, i) {
          const word& thisKey = thisKeys[matches[i]];
          entry& thisEntry =
            const_cast<entry&>(thisDict.lookupEntry(thisKey, false, false));
          if (addEntry
              (
                thisDict,
                thisEntry,
                mergeIter(),
                literalRE,
                HashTable<wordList, word>{0}  // no shortcuts at deeper levels
              )) {
            changed = true;
          }
        }
      }
    }
  }
  return changed;
}


int main(int argc, char *argv[])
{
  #include "add_dict_option.inc"
  argList::addOption
  (
    "instance",
    "name",
    "override instance setting (default is the time name)"
  );
  // Add explicit time option
  timeSelector::addOptions();
  argList::addBoolOption
  (
    "literalRE",
    "treat regular expressions literally (i.e., as a keyword)"
  );
  argList::addBoolOption
  (
    "enableFunctionEntries",
    "enable expansion of dictionary directives - #include, #codeStream etc"
  );
  argList::addBoolOption
  (
    "disablePatchGroups",
    "disable matching keys to patch groups"
  );
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  // Optionally override controlDict time with -time options
  instantList times = timeSelector::selectIfPresent(runTime, args);
  if (times.size() < 1) {
    FATAL_ERROR_IN(args.executable())
      << "No times selected." << exit(FatalError);
  }
  runTime.setTime(times[0], 0);
  word instance = args.optionLookupOrDefault("instance", runTime.timeName());
  #include "create_named_mesh.inc"
  const bool literalRE = args.optionFound("literalRE");
  if (literalRE) {
    Info << "Not interpreting any regular expressions (RE)"
      << " in the changeDictionaryDict." << endl
      << "Instead they are handled as any other entry, i.e. added if"
      << " not present." << endl;
  }
  const bool enableEntries = args.optionFound("enableFunctionEntries");
  if (enableEntries) {
    Info << "Allowing dictionary preprocessing ('#include', '#codeStream')."
      << endl;
  }
  int oldFlag = entry::disableFunctionEntries;
  if (!enableEntries) {
    // By default disable dictionary expansion for fields
    entry::disableFunctionEntries = 1;
  }
  const bool disablePatchGroups = args.optionFound("disablePatchGroups");
  if (disablePatchGroups) {
    Info << "Not interpreting any keys in the changeDictionary"
      << " as patchGroups"
      << endl;
  }
  fileName regionPrefix = "";
  if (regionName != fvMesh::defaultRegion) {
    regionPrefix = regionName;
  }
  // Make sure we do not use the master-only reading since we read
  // fields (different per processor) as dictionaries.
  regIOobject::fileModificationChecking = regIOobject::timeStamp;
  // Get the replacement rules from a dictionary
  const word dictName{"changeDictionaryDict"};
  #include "set_system_mesh_dictionary_io.inc"
  IOdictionary dict{dictIO};
  const dictionary& replaceDicts = dict.subDict("dictionaryReplacement");
  Info << "Read dictionary " << dict.name()
    << " with replacements for dictionaries "
    << replaceDicts.toc() << endl;
  // Always read boundary to get patch groups
  Info << "Reading polyMesh/boundary file to extract patch names"
    << endl;
  // Read PtrList of dictionary as dictionary.
  const word oldTypeName = IOPtrList<entry>::typeName;
  const_cast<word&>(IOPtrList<entry>::typeName) = word::null;
  IOPtrList<entry> dictList
  {
    // IOobject
    {
      "boundary",
      runTime.findInstance
      (
        regionPrefix/polyMesh::meshSubDir,
        "boundary",
        IOobject::READ_IF_PRESENT
      ),
      polyMesh::meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    }
  };
  const_cast<word&>(IOPtrList<entry>::typeName) = oldTypeName;
  // Fake type back to what was in field
  const_cast<word&>(dictList.type()) = dictList.headerClassName();
  // Temporary convert to dictionary
  dictionary fieldDict;
  FOR_ALL(dictList, i) {
    fieldDict.add(dictList[i].keyword(), dictList[i].dict());
  }
  if (dictList.size()) {
    Info << "Loaded dictionary " << dictList.name()
      << " with entries " << fieldDict.toc() << endl;
  }
  // Extract any patchGroups information (= shortcut for set of
  // patches)
  HashTable<wordList, word> patchGroups;
  if (!disablePatchGroups) {
    patchGroups = extractPatchGroups(fieldDict);
    if (patchGroups.size()) {
      Info << "Extracted patch groups:" << endl;
      wordList groups{patchGroups.sortedToc()};
      FOR_ALL(groups, i) {
        Info << "    group " << groups[i] << " with patches "
          << patchGroups[groups[i]] << endl;
      }
    }
  }
  // Every replacement is a dictionary name and a keyword in this
  FOR_ALL_CONST_ITER(dictionary, replaceDicts, fieldIter) {
    const word& fieldName = fieldIter().keyword();
    Info << "Replacing entries in dictionary " << fieldName << endl;
    // Handle 'boundary' specially:
    // - is PtrList of dictionaries
    // - is in polyMesh/
    if (fieldName == "boundary") {
      Info << "Special handling of " << fieldName
        << " as polyMesh/boundary file." << endl;
      // Get the replacement dictionary for the field
      const dictionary& replaceDict = fieldIter().dict();
      Info << "Merging entries from " << replaceDict.toc() << endl;
      // Merge the replacements in
      merge(fieldDict, replaceDict, literalRE, patchGroups);
      Info << "fieldDict:" << fieldDict << endl;
      // Convert back into dictList
      wordList doneKeys{dictList.size()};
      label nEntries = fieldDict.size();
      FOR_ALL(dictList, i) {
        doneKeys[i] = dictList[i].keyword();
        dictList.set
        (
          i,
          fieldDict.lookupEntry
          (
            doneKeys[i],
            false,
            true
          ).clone()
        );
        fieldDict.remove(doneKeys[i]);
      }
      // Add remaining entries
      label sz = dictList.size();
      dictList.setSize(nEntries);
      FOR_ALL_CONST_ITER(dictionary, fieldDict, iter) {
        dictList.set(sz++, iter().clone());
      }
      Info << "Writing modified " << fieldName << endl;
      dictList.writeObject
      (
        runTime.writeFormat(),
        runTime.writeFormat(),
        IOstream::UNCOMPRESSED
      );
    } else {
      // Read dictionary. (disable class type checking so we can load
      // field)
      Info << "Loading dictionary " << fieldName << endl;
      const word oldTypeName = IOdictionary::typeName;
      const_cast<word&>(IOdictionary::typeName) = word::null;
      IOdictionary fieldDict
      {
        {
          fieldName,
          instance,
          mesh,
          IOobject::MUST_READ_IF_MODIFIED,
          IOobject::NO_WRITE,
          false
        }
      };
      const_cast<word&>(IOdictionary::typeName) = oldTypeName;
      // Fake type back to what was in field
      const_cast<word&>(fieldDict.type()) = fieldDict.headerClassName();
      Info << "Loaded dictionary " << fieldName
        << " with entries " << fieldDict.toc() << endl;
      // Get the replacement dictionary for the field
      const dictionary& replaceDict = fieldIter().dict();
      Info << "Merging entries from " << replaceDict.toc() << endl;
      // Merge the replacements in
      merge(fieldDict, replaceDict, literalRE, patchGroups);
      Info << "Writing modified fieldDict " << fieldName << endl;
      fieldDict.regIOobject::write();
    }
  }
  entry::disableFunctionEntries = oldFlag;
  Info << endl;
  Info << "End\n" << endl;
  return 0;
}

