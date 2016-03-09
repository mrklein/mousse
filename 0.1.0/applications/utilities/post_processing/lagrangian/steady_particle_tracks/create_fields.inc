const word dictName("particleTrackDict");
#include "set_constant_mesh_dictionary_io.inc"
IOdictionary propsDict(dictIO);
word cloudName(propsDict.lookup("cloudName"));
List<word> userFields(propsDict.lookup("fields"));
