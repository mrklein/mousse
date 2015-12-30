#!/usr/bin/env python
# -*- coding: utf-8 -*-


from __future__ import print_function


HEADER1 = """\
// mousse: CFD toolbox
// Copyright (C) {0}-{1} OpenFOAM Foundation
// Copyright (C) 2016 mousse project
{2}
{3}

"""

HEADER2 = """\
// mousse: CFD toolbox
// Copyright (C) {0} OpenFOAM Foundation
// Copyright (C) 2016 mousse project
{1}
{2}

"""


def _camel_to_snake(name, sep='_'):
    """Convert CamelCase to snake_case.

    Solution from http://goo.gl/ulLL8o
    """
    from re import sub

    s1 = sub('(.)([A-Z][a-z]+)', r'\1{0}\2'.format(sep), name)
    return sub('([a-z0-9])([A-Z])', r'\1{0}\2'.format(sep), s1).lower()


def _backup_file(filename, prefix='', suffix='~'):
    """Find available backup file name and copy file."""
    from os.path import exists
    from shutil import copy

    backup_filename = prefix + filename + suffix
    cnt = 1
    while exists(backup_filename):
        backup_filename = '{0}~{1}'.format(filename, cnt)
        cnt += 1

    copy(filename, backup_filename)
    return backup_filename


def _reduce_indent(line, factor):
    """Reduce :line: indent by a factor :factor:."""
    indent = len(line) - len(line.lstrip())
    indent /= factor
    return ' '*indent + line.lstrip()


def _fix_includes(text):
    """Adapt include file names to new naming scheme."""
    from re import compile, M

    def __repl(m):
        return '#{0}include "{1}"'.format(m.group(1),
                                          _fix_filename(m.group(2)))

    r = compile(r'#(\s*)include\s+"(.*)"', M)

    return r.sub(__repl, text)


def _fix_include_guards(src, old_filename, new_filename):
    from os.path import splitext
    fn, fext = splitext(old_filename)
    old_guard = fn + '_' + fext[1:]
    fn, fext = splitext(new_filename)
    new_guard = fn + '_' + fext[1:] + '_'
    return src.replace(old_guard, new_guard)


def _fix_comments(text):
    """Remove extra comments, parse copyright years."""
    from re import compile, M, S
    from os import linesep

    # Useless star comments
    r1 = compile(r'\/\/ *(\* )+\/\/\n', M)
    # OpenFOAM foundation copyright years
    r2 = compile(r'Copyright \(C\) (\d+)-?(\d*)? OpenFOAM Foundation', M)
    # Head comment
    r3 = compile(r'\/\*-+\*\\(.*?)\\*-+\*\/\n', M | S)
    # Another star comments
    r4 = compile(r'\/\/ \*+ \/\/\n', M)
    r4v = compile(r'\/\/ \*+ \/\/', M)
    # "Bookmark" comments
    r5 = compile(r'\/\/ (\* )+((\w|\s)+)( \*)+ \/\/')

    t = r1.sub(r'', text)
    m1 = r2.search(t)
    m2 = r3.search(t)
    t = r3.sub(r'', t)
    t = r4.sub(r'', t)
    t = r4v.sub(r'', t)
    t = r5.sub(r'// \g<2>', t)

    doc_lines = []
    do_add = False
    do_fix_name = False
    if m2 is not None:
        for l in m2.group(1).splitlines():
            if len(l.strip()) == 0:
                continue
            if do_fix_name:
                doc_lines.append('//   ' + _fix_filename(l.strip()))
                continue
            if l.startswith('SourceFiles'):
                do_fix_name = True
            if do_add:
                doc_lines.append('// ' + _reduce_indent(l, 1))
                continue
            if l.startswith('Class'):
                doc_lines.append('// ' + _reduce_indent(l, 1))
                do_add = True
        if len(doc_lines) > 0:
            doc_lines.pop()
    docs = linesep.join(doc_lines)

    if m1 is not None:
        if len(m1.group(2)) > 0:
            t = HEADER1.format(m1.group(1), m1.group(2), docs, t)
        else:
            t = HEADER2.format(m1.group(1), docs, t)

    return t.rstrip()


def _fix_namespace(text):
    from re import compile, M
    r1 = compile(r'Foam::', M)
    r2 = compile(r'namespace\s+Foam', M)
    r3 = compile(r'}\s+\/\/\s+End\s+namespace', M)
    t1 = r1.sub(r'mousse::', text)
    t2 = r2.sub(r'namespace mousse', t1)
    t3 = r3.sub(r'}  // namespace', t2)
    return t3


def _fix_filename(filename):
    """Convert from camelCase to snake_case, fix extension."""
    from os.path import splitext, exists

    ext_map = {'.c': '.cpp', '.h': '.hpp'}

    t = _camel_to_snake(filename)
    name, ext = splitext(t)

    if ext in ext_map:
        ext = ext_map[ext]
    new_name = name + ext

    cnt = 1
    while exists(new_name):
        new_name = '{0}_{1}{2}'.format(name, cnt, ext)
        cnt += 1

    return new_name


def _squize(src):
    """Remove empty lines from :src:, reduce indent from 4 to 2 spaces"""
    import os
    res = []
    for l in src.splitlines():
        if not l:
            continue
        else:
            res.append(_reduce_indent(l, 2))
    return os.linesep.join(res)


def _fix_files_and_options(files=None, options=None):
    """Convert filenames to snake_case, fix library names in options."""
    from os.path import dirname
    if files is not None:
        with open(files, 'r') as f:
            output = ''
            for l in f:
                if l.startswith('LIB = '):
                    libname = l.split('=')[1][:-1]
                    libpath = dirname(libname)
                    libname = libname.strip().split('/')[-1]
                    libname = 'libmousse_' + _camel_to_snake(libname[3:])
                    output += 'LIB = {0}/{1}'.format(libpath, libname)
                elif l.startswith('EXE = '):
                    exename = l.split('=')[1][:-1]
                    exepath = dirname(exename)
                    exename = exename.strip().split('/')[-1]
                    exename = exename.replace('Foam', '')
                    exename = _camel_to_snake(exename, sep='-')
                    output += 'EXE = {0}/mousse-{1}'.format(exepath, exename)
                else:
                    if l == '\n':
                        output += l
                        continue
                    parts = l[:-1].split('/')
                    parts[-1] = _fix_filename(parts[-1])
                    parts[:-1] = [_camel_to_snake(p) for p in parts[:-1]]
                    output += '/'.join(parts) + '\n'
        with open(files, 'w') as f:
            f.write(output)

    if options is not None:
        with open(options, 'r') as f:
            output = ''
            for l in f:
                output += l
        with open(options, 'w') as f:
            f.write(output)


def _fix(filename='pimpleFoam.C', verbose=False):
    """Covert file into new naming scheme, correct includes and comments."""
    from shutil import move
    from os.path import basename, dirname, join, exists, splitext

    if verbose:
        print('Fixing {0}'.format(filename))
    _backup_file(filename)

    base = basename(filename)
    folder = dirname(filename)

    new_base = _fix_filename(base)
    new_filename = join(folder, new_base)

    n = 1
    new_name, new_ext = splitext(new_base)
    while exists(new_filename):
        new_filename = join(folder, '{0}_{1}{2}'.format(new_name, n, new_ext))

    move(filename, new_filename)
    with open(new_filename, 'r') as f:
        src = f.read()
    src = _squize(src)
    src = _fix_comments(src)
    src = _fix_includes(src)
    src = _fix_namespace(src)
    src = _fix_include_guards(src, base, new_base)

    with open(new_filename, 'w') as f:
        f.write(src + '\n')


def _transform(path, verbose=False):
    from os import walk, rename
    from os.path import splitext, join
    if verbose:
        print('Transforming {0} ... '.format(path))
    for root, dirs, files in walk(path, topdown=False, followlinks=True):
        for name in files:
            fn, fext = splitext(name)
            if fn == 'files':
                _fix_files_and_options(files=join(root, name))
                continue

            if fn == 'options':
                _fix_files_and_options(options=join(root, name))
                continue

            if fext == '.C' or fext == '.H':
                _fix(join(root, name))
                continue

            if fext == '':
                new_name = _camel_to_snake(name, sep='-')
                try:
                    rename(join(root, name), join(root, new_name))
                except OSError:
                    print('! {0}'.format(name))
                continue

        for name in dirs:
            if name == 'Make':
                new_name = '_make'
            else:
                new_name = _camel_to_snake(name)
            try:
                rename(join(root, name), join(root, new_name))
            except OSError:
                new_name += '_1'
                try:
                    rename(join(root, name), join(root, new_name))
                except OSError:
                    if verbose:
                        print('! {0}'.format(name))


def _run():
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('-V', '--verbose', action='store_true', dest='verbose',
                        help='Verbose output')
    parser.add_argument('path', action='store', type=str, nargs='+',
                        help='Path where to fix files')
    args = parser.parse_args()
    for path in args.path:
        _transform(path, args.verbose)

if __name__ == '__main__':
    _run()
