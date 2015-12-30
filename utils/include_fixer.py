#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Fix misspelled include files."""


def _fix_file(path, backup=True):
    from shutil import copy
    fixes = [
        ('i_ostream.hpp', 'iostream.hpp'),
        ('reg_i_oobject.hpp', 'reg_ioobject.hpp'),
        ('o_fstream.hpp', 'ofstream.hpp'),
        ('i_oobject_list.hpp', 'ioobject_list.hpp'),
        ('i_omanip.hpp', 'iomanip.hpp'),
        ('two_d_point_corrector.hpp', 'twod_point_corrector.hpp'),
        ('i_odictionary.hpp', 'iodictionary.hpp'),
        ('i_oobject.hpp', 'ioobject.hpp'),
        ('vector2_d.hpp', 'vector_2d.hpp'),
        ('i_fstream.hpp', 'ifstream.hpp'),
        ('io_field.hpp', 'iofield.hpp'),
        ('compact_iofield.hpp', 'compact_io_field.hpp'),
        ('i_ostreams.hpp', 'iostreams.hpp'),
        ('nsrd_sfunc0.hpp', 'nsrds_func0.hpp'),
        ('nsrd_sfunc1.hpp', 'nsrds_func1.hpp'),
        ('nsrd_sfunc2.hpp', 'nsrds_func2.hpp'),
        ('nsrd_sfunc3.hpp', 'nsrds_func3.hpp'),
        ('nsrd_sfunc4.hpp', 'nsrds_func4.hpp'),
        ('nsrd_sfunc5.hpp', 'nsrds_func5.hpp'),
        ('nsrd_sfunc6.hpp', 'nsrds_func6.hpp'),
        ('nsrd_sfunc7.hpp', 'nsrds_func7.hpp'),
        ('nsrd_sfunc14.hpp', 'nsrds_func14.hpp'),
        ('ap_idiff_coef_func.hpp', 'api_diff_coef_func.hpp'),
        ('scalar_iofield.hpp', 'scalar_io_field.hpp'),
        ('io_list.hpp', 'iolist.hpp')
    ]
    if backup:
        backup_path = path + '~'
        try:
            copy(path, backup_path)
        except OSError as e:
            print('Can not backup file: {0}'.format(path))
            print(str(e))
    with open(path, 'r') as f:
        src = f.read()
        for fix in fixes:
            a, b = fix
            src = src.replace(a, b)
    with open(path, 'w') as f:
        f.write(src)


def _fix_includes():
    from argparse import ArgumentParser
    from os import walk
    from os.path import splitext, join
    exts_for_fix = ['.hpp', '.cpp']
    parser = ArgumentParser()
    parser.add_argument('-p', '--path', action='store', type=str,
                        dest='path', default='.')
    parser.add_argument('-R', '--no-recursion', action='store_false',
                        dest='resurse')
    parser.add_argument('-i', '--inplace', action='store_false',
                        dest='backup')
    args = parser.parse_args()
    for root, dirs, files in walk(args.path, topdown='False',
                                  followlinks=True):
        for name in files:
            fn, fext = splitext(name)
            if fext in exts_for_fix:
                _fix_file(join(root, name), backup=args.backup)


if __name__ == '__main__':
    _fix_includes()
