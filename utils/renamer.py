#!/usr/bin/env python
# -*- coding: utf-8 -*-


from __future__ import print_function
from os import rename, listdir, walk
from os.path import basename, dirname, join, isdir, exists, isfile
from unittest import TestCase


class NameTransformer(object):
    def __init__(self, from_="camel", to="snake"):
        self._from = from_
        self._to = to

    @staticmethod
    def camel2snake(name):
        import re
        _underscorer1 = re.compile(r'(.)([A-Z][a-z]+)')
        _underscorer2 = re.compile('([a-z0-9])([A-Z])')
        subbed = _underscorer1.sub(r'\1_\2', name)
        return _underscorer2.sub(r'\1_\2', subbed).lower()

    @staticmethod
    def camel2title(name):
        return name[0].upper() + name[1:]

    @staticmethod
    def camel2upper(name):
        c2s = getattr(NameTransformer, "camel2snake")
        s2u = getattr(NameTransformer, "snake2upper")
        return s2u(c2s(name))

    @staticmethod
    def snake2camel(name):
        res = []
        for i, n in enumerate(name.split("_")):
            if i == 0:
                res.append(n.lower())
            else:
                res.append(n.title())
        return ''.join(res)

    @staticmethod
    def snake2title(name):
        return ''.join([n.title() for n in name.split("_")])

    @staticmethod
    def snake2upper(name):
        return "_".join([n.upper() for n in name.split("_")])

    @staticmethod
    def upper2snake(name):
        return name.lower()

    @staticmethod
    def upper2camel(name):
        u2s = getattr(NameTransformer, "upper2snake")
        s2c = getattr(NameTransformer, "snake2camel")
        return s2c(u2s(name))

    @staticmethod
    def upper2title(name):
        pass

    def __call__(self, name):
        m = getattr(NameTransformer,
                    '{0}2{1}'.format(self._from, self._to))
        if m is None:
            raise NotImplementedError
        return m(name)


def _get_files(path, recurse=False):
    """Get files in the folder optionally recursing into sub-folders.

    Parameters:
    :path string: path to the folder
    :recurse boolean: whether to recurse into sub-folders
    """
    if not isdir(path):
        raise RuntimeError('path MUST be directory')
    if not exists(path):
        raise RuntimeError('path does not exist')
    if not recurse:
        for itm in (join(path, itm) for itm in listdir(path) if isfile(itm)):
            yield itm
    else:
        for root, _, files in walk(path, topdown=False, followlinks=False):
            for itm in files:
                yield join(root, itm)


def _get_directories(path, recurse=False):
    """Get directories in the folder optionally recursing into sub-folders.

    Parameters:
    :path string: path to the folder
    :recurse boolean: whether to recurse into sub-folders
    """
    if not isdir(path):
        raise RuntimeError('path MUST be directory')
    if not exists(path):
        raise RuntimeError('path does not exist')
    if not recurse:
        for d in (join(path, itm) for itm in listdir(path) if isdir(itm)):
            yield d
    else:
        for root, dirs, _ in walk(path, topdown=False, followlinks=False):
            for d in dirs:
                yield join(root, d)


def _rename_files_in(paths, recurse=False, only_directories=False,
                     only_files=False, from_="camel", to="snake"):
    transformer = NameTransformer(from_, to)

    def rename_item(name):
        base = basename(name)
        folder = dirname(name)
        new_name = join(folder, transformer(base))
        try:
            rename(name, new_name)
        except OSError:
            print("Can not rename {0}".format(name))

    for itm in paths:
        if not only_directories:
            for f in _get_files(itm, recurse):
                rename_item(f)

        if not only_files:
            for d in _get_directories(itm, recurse):
                rename_item(d)


class NameTransformerTest(TestCase):
    def test_snake_to_camel(self):
        t = NameTransformer("snake", "camel")
        self.assertEqual(t("test_string1"), "testString1")
        self.assertEqual(t("test_string2_with_addition"),
                         "testString2WithAddition")

    def test_snake_to_upper(self):
        t = NameTransformer("snake", "upper")
        self.assertEqual(t("test_string1"), "TEST_STRING1")
        self.assertEqual(t("test_string2_with_addition"),
                         "TEST_STRING2_WITH_ADDITION")

    def test_snake_to_title(self):
        t = NameTransformer("snake", "title")
        self.assertEqual(t("test_string1"), "TestString1")
        self.assertEqual(t("test_string2_with_addition"),
                         "TestString2WithAddition")

    def test_camel_to_snake(self):
        t = NameTransformer("camel", "snake")
        self.assertEqual(t("testString"), "test_string")
        self.assertEqual(t("testIOString"), "test_io_string")
        self.assertEqual(t("testString2WithNumber"),
                         "test_string2_with_number")

    def test_camel_to_upper(self):
        t = NameTransformer("camel", "upper")
        self.assertEqual(t("testString"), "TEST_STRING")
        self.assertEqual(t("testIOString"), "TEST_IO_STRING")
        self.assertEqual(t("testString2WithNumber"),
                         "TEST_STRING2_WITH_NUMBER")

    def test_camel_to_title(self):
        t = NameTransformer("camel", "title")
        self.assertEqual(t("testString"), "TestString")
        self.assertEqual(t("testIOString"), "TestIOString")
        self.assertEqual(t("testString2WithNumber"),
                         "TestString2WithNumber")


def _rename():
    from argparse import ArgumentParser

    parser = ArgumentParser()
    parser.add_argument("-r", "--recurse", help="Recurse into directories",
                        action="store_true")
    parser.add_argument("-D", "--directories", help="Rename only directories",
                        action="store_true")
    parser.add_argument("-F", "--files", help="Rename only files",
                        action="store_true")
    parser.add_argument("-f", "--from", dest="from_",
                        help="Rename just items that have special format",
                        choices=["snake", "upper", "title", "camel"],
                        default="camel")
    parser.add_argument("-t", "--to", help="Rename into given format",
                        choices=["snake", "upper", "title", "camel"],
                        default="snake")
    parser.add_argument("paths", action="store", nargs="*", default=["."])
    args = parser.parse_args()

    _rename_files_in(args.paths, recurse=args.recurse,
                     only_directories=args.directories, only_files=args.files,
                     from_=args.from_, to=args.to)

if __name__ == '__main__':
    _rename()
