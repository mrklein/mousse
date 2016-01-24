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


def _is_case_folder(path):
    """Check if :path: is case folder."""
    from os.path import isdir, join, exists

    if not isdir(path):
        return False

    # Check for common sub-folders
    res = True

    common_paths = ['system', 'constant', join('system', 'controlDict'),
                    join('system', 'fvSchemes'), join('system', 'fvSolution')]

    for itm in common_paths:
        res = res and exists(join(path, itm))

    return res


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


def _search_cases(path, recurse=False, verbose=False):
    """Search case folders in a given folder."""
    from os import walk, listdir
    from os.path import isdir, join

    if verbose:
        print('Searching for cases in {0}'.format(path))

    if not recurse:
        for itm in listdir(path):
            real_path = join(path, itm)
            if isdir(real_path) and _is_case_folder(real_path):
                _fix_case(real_path, verbose)
    else:
        for root, dirs, _ in walk(path, topdown=False, followlinks=True):
            for itm in dirs:
                real_path = join(root, itm)
                if _is_case_folder(real_path):
                    _fix_case(real_path, verbose)

    if verbose:
        print('Done.')


def _add_run_and_clean(path):
    from textwrap import dedent
    from os import chmod
    from os.path import join
    from stat import S_IRWXU, S_IXGRP, S_IRGRP, S_IXOTH, S_IROTH

    allrun = join(path, 'allrun')
    allclean = join(path, 'allclean')

    with open(allrun, 'w') as f:
        f.write(dedent(
            """\
            #!/bin/sh

            cd ${0%/*} || exit 1

            . $WM_PROJECT_DIR/bin/tools/run-functions

            run_application block-mesh
            run_application $(get_application)
            """
        ))

    with open(allclean, 'w') as f:
        f.write(dedent(
            """\
            #!/bin/sh

            cd ${0%/*} || exit 1

            . $WM_PROJECT_DIR/bin/tools/clean-functions

            clean_case
            """
        ))

    chmod(allrun, S_IRWXU | S_IXGRP | S_IXOTH | S_IRGRP | S_IROTH)
    chmod(allclean, S_IRWXU | S_IXGRP | S_IXOTH | S_IRGRP | S_IROTH)


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


def _fix_case(path, verbose=False):
    """Correct case dictionaries."""
    from os.path import exists, isdir, join, basename, dirname
    from os import walk, rename

    if not exists(path):
        raise OSError

    if not isdir(path):
        raise OSError

    if verbose:
        print('  {0}'.format(path))

    parent = dirname(path)

    for root, dirs, files in walk(path, topdown=False, followlinks=True):
        for name in files:
            # Fix dictionaries
            if _is_foam_dictionary(join(root, name)):
                _fix_dictionary(join(root, name))
                if verbose:
                    t = root.replace(parent, '')[1:]
                    print('    fixing dictionary {0}'.format(join(t, name)))

            # Allrun, Allclean, and Alltest -> allrun, allclean, alltest
            if name in ['Allrun', 'Allclean', 'Alltest']:
                rename(join(root, name), join(root, name.lower()))

    # Move case into snake_case
    base = basename(path)
    parent = dirname(path)

    base = _camel_to_snake(base)
    rename(path, join(parent, base))
    if verbose:
        print('    move {0} -> {1}\n'.format(basename(path), base))

    # Add allrun and allclean files to the case folder
    # _add_run_and_clean(join(parent, base))


def _fix_cases():
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
        _search_cases(path, args.recurse, args.verbose)

if __name__ == '__main__':
    _fix_cases()
