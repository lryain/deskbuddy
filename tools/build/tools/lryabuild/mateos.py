#!/usr/bin/env python2

from __future__ import print_function

import argparse
import os
import platform
import re
import string
import subprocess
import sys

# lryabuild
import toolget


def get_lrya_downloads_dir():
    return os.path.join(os.path.expanduser("~"), ".lrya", "mateos", "mateos-downloads")


def get_lrya_dist_dir():
    return os.path.join(os.path.expanduser("~"), ".lrya", "mateos", "mateos-repository")


def get_version_tag(version):
    return "mateos-sdk-{}".format(version)


def install_mateos_sdk(version):
    platform_map = {"darwin": "x86_64-apple-darwin", "linux": "x86_64-linux-gnu"}

    platform_name = platform.system().lower()

    subver_tag = ""
    if platform_name == "linux":
        import lsb_release

        subver = lsb_release.get_lsb_information().get("RELEASE")
        subver_tag = "-ubuntu-{}".format(subver)

    platform_tag = "{}{}".format(platform_map.get(platform_name), subver_tag)

    url_prefix = "https://sai-general.s3.amazonaws.com/build-assets/"
    platform_name = platform.system().lower()
    sdk_base_name = "mateos-sdk-{0}-{1}".format(version, platform_tag)
    lrya_sdk_url = "{0}{1}.tar.bz2".format(url_prefix, sdk_base_name)
    lrya_hash_url = "{0}{1}-SHA-256.txt".format(url_prefix, sdk_base_name)

    downloads_path = toolget.get_lrya_tool_downloads_directory("mateos-sdk")
    dist_path = toolget.get_lrya_tool_dist_directory("mateos-sdk")

    # mateos_basename = get_version_tag(version)
    mateos_basename = sdk_base_name
    # downloads_path = get_lrya_downloads_dir()
    # dist_path = os.path.join(get_lrya_dist_dir(), get_version_tag(version))
    toolget.download_and_install(
        lrya_sdk_url,
        lrya_hash_url,
        downloads_path,
        dist_path,
        mateos_basename,
        mateos_basename,
        version,
        "mateos-sdk",
    )


def get_sdk_version_from_sdk_dir(sdk_dir):
    version_file = os.path.join(sdk_dir, "VERSION")
    version = None
    with open(version_file, "r") as f:
        version = f.read().strip()
    return version


def get_lrya_sdk_dir(required_ver):
    # check for a version in .lrya
    # mateos_sdk_tag = get_version_tag(required_ver)
    lrya_mateos_base_dir = toolget.get_lrya_tool_dist_directory("mateos-sdk")
    lrya_mateos_dir = os.path.join(lrya_mateos_base_dir, required_ver)
    return lrya_mateos_dir


def find_sdk_root_dir(required_ver):
    env_value = os.environ.get("MATEOS_SDK_HOME")
    if env_value:
        # check environment defined value
        version = get_sdk_version_from_sdk_dir(env_value)
        if version == required_ver:
            return env_value

    # check for a version in .lrya
    lrya_mateos_dir = get_lrya_sdk_dir(required_ver)

    if not os.path.exists(lrya_mateos_dir):
        return None
    else:
        return lrya_mateos_dir


def find_or_install_mateos_sdk(required_ver, install=True):
    sdk_root_dir = find_sdk_root_dir(required_ver)

    if sdk_root_dir:
        return sdk_root_dir

    if install:
        # install_mateos_sdk(required_ver)
        print("--------> in mateos.py Skipping install_mateos_sdk ", required_ver)
    sdk_root_dir = find_sdk_root_dir(required_ver)

    print("--------> in mateos.py MATEOS Already exist! version is: ", required_ver)

    return sdk_root_dir


def parseArgs(scriptArgs):
    version = "1.0"
    parser = argparse.ArgumentParser(
        description="finds or installs mateos sdk", version=version
    )
    parser.add_argument("--install", action="store", dest="install_version", nargs="?")
    parser.add_argument("--find", action="store", dest="find_version", nargs="?")
    (options, args) = parser.parse_known_args(scriptArgs)
    return options


def main(argv):
    options = parseArgs(argv)
    version = options.install_version or options.find_version
    if version:
        path = find_or_install_mateos_sdk(version, bool(options.install_version))
        if not path:
            return 1
        print("%s" % path)
        return 0


if __name__ == "__main__":
    ret = main(sys.argv)
    sys.exit(ret)
