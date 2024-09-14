#!/usr/bin/python3

######################################################################
#  Add support to native system to the arkos emulator configuration.
#
#  This file is licensed under the BSD license.
#
#  2024-09-10 Tue
#  Dov Grobgeld <dov.grobgeld@gmail.com>
######################################################################

import argparse
import re
from pathlib import Path
import os

def find_roms_dir():
    '''Look for the rom files'''
    for candidate in ['/roms2',
                      '/roms']:
        if Path(candidate).exists():
            return candidate
    raise RuntimeError('Failed finding rom directory')

def insert_native_system(filename, roms_dir='/roms'):
    filename = Path(filename)
    with open(filename) as fh:
        ctx = fh.read()

    if re.search('<name>Native</name>', ctx):
        print('Native support already added!')
        return False

    native_system_ctx = f'''  <system>
    <name>Native</name>
    <fullname>Native</fullname>
    <path>{roms_dir}/native/</path>
    <extension>.exec .run .py .lua</extension>
    <command>/roms2/native/run.sh %ROM%</command>
    <platform>native</platform>
    <theme>native</theme>
  </system>

'''
    ctx = re.sub('</systemList>', native_system_ctx + r'\g<0>', ctx)
    filename.rename(filename.with_suffix('.cfg.bak'))
    with open(filename, 'w') as fh:
        fh.write(ctx)

    return True

def copy_theme(theme_source_dir, theme_dest_dir, override=False):
    theme_source_dir = Path(theme_source_dir)
    theme_dest_dir = Path(theme_dest_dir)

    if not theme_source_dir.exists():
        raise FileNotFoundError(f'Failed finding theme directory. Looked in: "{theme_source_dir}"')

    if theme_dest_dir.exists():
        if not override:
            raise RuntimeError('Destination theme directory already exists!')
    else:
        theme_dest_dir.mkdir(0o755)

    ret = os.system(f'rsync -av {theme_source_dir} {theme_dest_dir.parent}')
    if (ret>>8) > 0:
        raise RuntimeError('Failed rsyncing the theme directory')

def create_roms_directory(roms_dir):
    roms_dir = Path(roms_dir)
    if not roms_dir.exists():
        roms_dir.mkdir(0o755)
    os.system(f'rsync -av ../roms/native {roms_dir}')
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Install native "emulator"')
    parser.add_argument('--cfg-file',
                        dest='cfg_file',
                        action='store',
                        type=str,
                        default='/etc/emulationstation/es_systems.cfg',
                        help='cfg file to process')
    parser.add_argument('--roms-dir',
                        dest='roms_dir',
                        action='store',
                        type=str,
                        default=None,
                        help='Where the roms are stored')
    parser.add_argument('--theme-source-dir',
                        dest='theme_source_dir',
                        action='store',
                        type=str,
                        default='../themes/native',
                        help='Where is the theme')
    parser.add_argument('--theme-dest-dir',
                        dest='theme_dest_dir',
                        action='store',
                        type=str,
                        default='/etc/emulationstation/themes/es-theme-nes-box/native',
                        help='Where should the theme be stored')
    parser.add_argument('--override',
                        dest='override',
                        action='store_true',
                        help='Override already existing theme')
    parser.add_argument('--no-reboot',
                        dest='no_reboot',
                        action='store_true',
                        help='Don\'t reboot when done')
    args = parser.parse_args()

    roms_dir = args.roms_dir
    cfg_file = args.cfg_file
    theme_source_dir = args.theme_source_dir
    theme_dest_dir = args.theme_dest_dir
    override = args.override

    if roms_dir is None:
        roms_dir = find_roms_dir()

    if insert_native_system(cfg_file, roms_dir=roms_dir):
        print(f'Succesfully modified {cfg_file}')

    copy_theme(theme_source_dir=theme_source_dir,
               theme_dest_dir=theme_dest_dir,
               override=override)

    if not args.no_reboot:
        os.system('sudo reboot')
