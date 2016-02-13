#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function


def _camel_to_snake(name, sep='_'):
    """Convert CamelCase to snake_case.

    Solution from http://goo.gl/ulLL8o
    """
    from re import sub

    s1 = sub('(.)([A-Z][a-z]+)', r'\1{0}\2'.format(sep), name)
    return sub('([a-z0-9])([A-Z])', r'\1{0}\2'.format(sep), s1).lower()


def _reduce_indent(line, factor):
    """Reduce :line: indent by a factor :factor:."""
    indent = len(line) - len(line.lstrip())
    indent /= factor
    return ' '*indent + line.lstrip()


def _is_foam_dictionary(path):
    """Check if :path: is Foam dictionary file."""
    from os.path import exists, isfile
    from re import compile, DOTALL
    if not exists(path):
        return False
    if not isfile(path):
        return False

    foam_file = compile('FoamFile.*?\{.*?version\s+(.+);.*\}', DOTALL)
    with open(path, 'r') as f:
        src = f.read()
        return foam_file.search(src) is not None


def _search_dictionaries(path, recurse=False, verbose=False):
    """Search case folders in a given folder."""
    from os import walk, listdir
    from os.path import isfile, join

    if verbose:
        print('Searching for cases in {0}'.format(path))

    if not recurse:
        for itm in listdir(path):
            real_path = join(path, itm)
            if isfile(real_path) and _is_foam_dictionary(real_path):
                _fix_dictionary(real_path)
    else:
        for root, _, files in walk(path, topdown=False, followlinks=True):
            for itm in files:
                real_path = join(root, itm)
                if _is_foam_dictionary(real_path):
                    _fix_dictionary(real_path)

    if verbose:
        print('Done.')


def _fix_dictionary(path):
    from os.path import exists, isfile, basename
    from re import compile, M, S
    from os import linesep

    if not exists(path):
        raise OSError

    if not isfile(path):
        raise OSError

    src = ''
    with open(path, 'r') as f:
        src = f.read()
        # Useless star comments
        r1 = compile(r'\/\/ *(\* )+\/\/\n', M)
        # Head comment=
        r2 = compile(r'\/\*-+\*- C\+\+ -\*-+\*\\(.*?)\\*-+\*\/\n', M | S)
        # Another star comments
        r3 = compile(r'\/\/ \*+ \/\/\n', M)
        r3v = compile(r'\/\/ \*+ \/\/', M)

        src = r1.sub(r'', src)
        src = r2.sub(r'', src)
        src = r3.sub(r'', src)
        src = r3v.sub(r'', src)

    # In controlDict fix application settings
    if basename(path) == 'controlDict':
        r1 = compile(r'(application\s+)(\w+)(\s*);', M)
        r2 = compile(r'(.*)Foam')
        m1 = r1.search(src)
        if m1 is not None:
            app = m1.group(2)
            m2 = r2.search(app)
            if m2 is not None:
                app = 'mousse-' + m2.group(1)
            src = r1.sub(m1.group(1) + app + m1.group(3) + ';\n', src)

    lines = src.split(linesep)
    reduced_indent = []
    header = False
    for l in lines:
        if len(l) < 1:
            continue
        if l.strip() == 'FoamFile':
            header = True
        if l.strip() == 'boundaryField':
            reduced_indent.append('')
        reduced_indent.append(_reduce_indent(l, 2))
        if l.strip() == '}' and header:
            reduced_indent.append('')
            header = False

    with open(path, 'w') as f:
        f.write('// mousse: CFD toolbox\n\n')
        for l in reduced_indent:
            f.write('{0}\n'.format(l))
        f.write('\n// vim: set ft=foam et sw=2 ts=2 sts=2:\n')


def _fix_dictionaries():
    """Parse arguments, collect case folders, fix dictionaries."""
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('-V', '--verbose', action='store_true',
                        help='Verbose output')
    parser.add_argument('-R', '--recurse', action='store_true',
                        help='Recurse into sub-folders searching for cases')
    parser.add_argument('paths', action='store', type=str, nargs='*',
                        help='Path to cases, if omitted search in current'
                        'folder')

    args = parser.parse_args()

    for path in args.paths:
        _search_dictionaries(path, args.recurse, args.verbose)

if __name__ == '__main__':
    _fix_dictionaries()
