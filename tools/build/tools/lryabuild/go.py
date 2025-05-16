#!/usr/bin/env python2
# -*- coding: utf-8 -*-

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

GO = 'go'
# DEFAULT_VERSION = '1.11'
# DEFAULT_VERSION = '1.12.17'
DEFAULT_VERSION = '1.13.8'

def get_go_version_from_command(go_exe):
    version = None
    if go_exe and os.path.exists(go_exe):
        output = subprocess.check_output([go_exe, 'version'])
        if not output:
            return None
        m = re.match('^go version go(\d+\.\d+\.?\d*)', output)
        if not m:
            return None
        version = m.group(1)
    version = DEFAULT_VERSION
    return version

def find_lrya_go_exe(version):
    d = toolget.get_lrya_tool_dist_directory(GO)
    d_ver = os.path.join(d, version)

    for root, dirs, files in os.walk(d_ver):
        if os.path.basename(root) == 'bin':
            if 'go' in files:
                return os.path.join(d_ver, root, 'go')
    return None

def install_go(version):
    platform_map = {
        'darwin': 'darwin-amd64',
        'linux': 'linux-arm64'
    }

    sha_map = {
        '1.9.4-darwin': '0e694bfa289453ecb056cc70456e42fa331408cfa6cc985a14edb01d8b4fec51',
        '1.9.4-linux': '15b0937615809f87321a457bb1265f946f9f6e736c563d6c5e0bd2c22e44f779',
        '1.10.1-darwin': '0a5bbcbbb0d150338ba346151d2864fd326873beaedf964e2057008c8a4dc557',
        '1.10.1-linux': '72d820dec546752e5a8303b33b009079c15c2390ce76d67cf514991646c6127b',
        '1.10.2-darwin': '360ad908840217ee1b2a0b4654666b9abb3a12c8593405ba88ab9bba6e64eeda',
        '1.10.2-linux': '4b677d698c65370afa33757b6954ade60347aaca310ea92a63ed717d7cb0c2ff',
        '1.10.4-darwin': '2ba324f01de2b2ece0376f6d696570a4c5c13db67d00aadfd612adc56feff587',
        '1.10.4-linux': 'fa04efdb17a275a0c6e137f969a1c4eb878939e91e1da16060ce42f02c2ec5ec',
        '1.11-darwin': '9749e6cb9c6d05cf10445a7c9899b58e72325c54fee9783ed1ac679be8e1e073',
        '1.11-linux': 'b3fcf280ff86558e0559e185b601c9eade0fd24c900b4c63cd14d1d38613e499',
        # armv6l https://golang.google.cn/dl/go1.13.8.linux-armv6l.tar.gz
        # '1.13.8-linux': '75f590d8e048a97cbf8b09837b15b3e6b44e1374718a96a5c3a994843ef44a4d'
        # go1.13.8.linux-arm64.tar.gz	Archive	Linux	ARM64	93MB	b46c0235054d0eb69a295a2634aec8a11c7ae19b3dc53556a626b89dc1f8cdb0
        # https://golang.google.cn/dl/go1.13.8.linux-arm64.tar.gz
        # arm64
        '1.13.8-linux': 'b46c0235054d0eb69a295a2634aec8a11c7ae19b3dc53556a626b89dc1f8cdb0'
        # '1.13.8-linux': '0567734d558aef19112f2b2873caa0c600f1b4a5827930eb5a7f35235219e9d8'
    }

    platform_name = platform.system().lower()

    go_platform = platform_map[platform_name]
    go_archive_url = "https://dl.google.com/go/go{}.{}.tar.gz".format(version, go_platform)
    print("----------------> Downloading go %s" % go_archive_url)
    go_basename = "go-{}-{}".format(version, go_platform)
    go_downloads_path = toolget.get_lrya_tool_downloads_directory(GO)
    go_dist_path = toolget.get_lrya_tool_dist_directory(GO)
    go_hash = sha_map['{}-{}'.format(version, platform_name)]

    if go_hash is None:
        print("Error: Don't know hash for %s" % go_basename)
        sys.exit(1)
# 如果没有安装go，下载并安装
# 检查go gz文件是否存在
    if not os.path.exists(os.path.join(go_downloads_path, go_basename + ".tar.gz")):
        print("Downloading %s" % go_archive_url)
        toolget.download_and_install(go_archive_url,
                                 go_hash,
                                 go_downloads_path,
                                 go_dist_path,
                                 go_basename,
                                 "go",
                                 version,
                                 "go")
    else:
        print("File %s already exists" % os.path.join(go_downloads_path, go_basename + ".tar.gz"))
    

def find_or_install_go(required_ver, go_exe=None):
    print("查找或者安装指定版本的go： %s" % required_ver)

    if not go_exe:
        try:
            # print("go.py ---------------> subprocess.check_output(['which', 'go'])")
            go_exe = subprocess.check_output(['which', 'go'])
            print("go_exe", go_exe)
        except subprocess.CalledProcessError as e:
            pass

    needs_install = True
    if go_exe:
        version = get_go_version_from_command(go_exe)
        if version == required_ver:
            needs_install = False

    if needs_install:
        go_exe = find_lrya_go_exe(required_ver)
        version = get_go_version_from_command(go_exe)
        if version == required_ver:
            needs_install = False

    if needs_install:
        print("没有检测到go，现在安装：Installing go version %s" % required_ver)
        install_go(required_ver)
        return find_lrya_go_exe(required_ver)
    else:
        return go_exe

def setup_go(required_ver):
    go_exe = find_or_install_go(required_ver)
    if not go_exe:
        raise RuntimeError("Could not find go version {0}"
                            .format(required_ver))
    return go_exe

def parseArgs(scriptArgs):
    version = '1.0'

    parser = argparse.ArgumentParser(description='finds or installs go', version=version)
    parser.add_argument('--install-go',
                        action='store',
                        dest='required_version',
                        nargs='?',
                        default=DEFAULT_VERSION)
    parser.add_argument('--check-version',
                        action='store',
                        dest='check_exe',
                        nargs=1)
    
    (options, args) = parser.parse_known_args(scriptArgs)
    return options


def main(argv):
    options = parseArgs(argv)

    if options.check_exe:
        exe = options.check_exe[0]
        checked_version = get_go_version_from_command(exe)
        if checked_version != DEFAULT_VERSION:
            print("Warning: custom Go installation at {} has version {}, desired is {}"
                .format(exe, checked_version, DEFAULT_VERSION))
        return 0
    if options.required_version:
        path = find_or_install_go(options.required_version)
        if not path:
            return 1
        print("%s" % path)
        return 0

if __name__ == '__main__':
    ret = main(sys.argv)
    sys.exit(ret)

