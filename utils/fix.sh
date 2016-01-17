#!/bin/bash

fix_files () {
    local search_pattern="$1"
    local sed_pattern="$2"

    local files=$(ack -l $search_pattern)
    [ -n "$files" ] && sed -i~ $sed_pattern $files
}

fix_files 'TypeName\(' 's%[[:<:]]TypeName(%TYPE_NAME(%g'
fix_files 'ClassName\(' 's%[[:<:]]ClassName(%CLASS_NAME(%g'
fix_files 'FatalErrorIn' 's%[[:<:]]FatalErrorIn%FATAL_ERROR_IN%g'
fix_files 'SeriousErrorIn' 's%[[:<:]]SeriousErrorIn%SERIOUS_ERROR_IN%g'
fix_files 'WarningInFunction' 's%[[:<:]]WarningInFunction%WARNING_IN_FUNCTION%g'
fix_files 'WarningIn' 's%[[:<:]]WarningIn%WARNING_IN%g'
fix_files 'FatalIOErrorIn' 's%[[:<:]]FatalIOErrorIn%FATAL_IO_ERROR_IN%g'
fix_files 'notImplemented' 's%notImplemented%NOT_IMPLEMENTED%g'
fix_files 'declareRunTimeSelectionTable' \
    's%declareRunTimeSelectionTable%DECLARE_RUN_TIME_SELECTION_TABLE%g'
fix_files 'defineTypeNameAndDebug' \
    's%defineTypeNameAndDebug%DEFINE_TYPE_NAME_AND_DEBUG%g'
fix_files 'defineRunTimeSelectionTable' \
    's%defineRunTimeSelectionTable%DEFINE_RUN_TIME_SELECTION_TABLE%g'
fix_files 'defineTemplateTypeNameAndDebug' \
    's%defineTemplateTypeNameAndDebug%DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG%g'
fix_files 'defineNamedTemplateTypeNameAndDebug' \
    's%defineNamedTemplateTypeNameAndDebug%DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG%g'
fix_files 'forAll\(' 's%forAll(%FOR_ALL(%g'
fix_files 'forAllReverse' 's%[[:<:]]forAllReverse%FOR_ALL_REVERSE%g'
fix_files 'forAllConstIter' 's%forAllConstIter%FOR_ALL_CONST_ITER%g'
fix_files 'forAllIter' 's%forAllIter%FOR_ALL_ITER%g'
fix_files 'addToRunTimeSelectionTable' \
    's%addToRunTimeSelectionTable%ADD_TO_RUN_TIME_SELECTION_TABLE%g'
fix_files 'makePatchFields' 's%makePatchFields%MAKE_PATCH_FIELDS%g'

