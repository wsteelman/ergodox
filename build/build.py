#!/user/local/bin python3.5
import argparse
import os
import shutil
import glob
import subprocess

def generate_cmake_cmd(location, opts):
   cmd = 'cmake {cmake_dir} '.format(cmake_dir=location)
   for key, val in opts.items():
      cmd += "'-D{opt}={val}' ".format(opt=key, val=val)
   return cmd

def build(name, opts, cli_args):
   print('\nBuilding {name}...'.format(name=name))
   # create and cd to output directory
   os.chdir(cli_args.output_dir)
   if os.path.exists(name):
      shutil.rmtree(name)
   os.makedirs(name, exist_ok=True)
   os.chdir(name)

   # copy KLL files to output dir
   for filename in glob.glob('{kll_dir}/*'.format(kll_dir=cli_args.kll_dir)):
      shutil.copy(filename, './')

   cmd = generate_cmake_cmd(cli_args.cmake_dir, opts)
   print(cmd)
   exit_code = subprocess.call(cmd, shell=True)
   if exit_code:
      print('ERROR: failed to cmake {name}'.format(name=name))
      return False

   exit_code = subprocess.call("make")
   if exit_code:
      print('ERROR: failed to make {name}'.format(name=name))
      return False

   return True

# keyboard specific cmake options
keyboards = {
   'ergodox-left' : {
      'CHIP' : 'mk20dx256vlh7',
      'ScanModule' : 'Infinity_Ergodox',
      'BaseMap' : 'scancode_map leftHand slave1 rightHand'
      },
   'ergodox-right' : {
      'CHIP' : 'mk20dx256vlh7',
      'ScanModule' : 'Infinity_Ergodox',
      'BaseMap' : 'scancode_map rightHand slave1 leftHand'
      },
   'whitefox' : {
      'CHIP' : 'mk20dx256vlh7',
      'ScanModule' : 'WhiteFox',
      'BaseMap' : 'scancode_map'
      },
   'infinity-60' : {
      'CHIP' : 'mk20dx128vlf5',
      'ScanModule' : 'Infinity_60%',
      'BaseMap' : 'scancode_map'
      },
   'infinity-60-led' : {
      'CHIP' : 'mk20dx128vlf5',
      'ScanModule' : 'Infinity_60%_LED',
      'BaseMap' : 'scancode_map'
      },

}

# default/shared cmake options
default_cmake_opts = {
   'COMPILER'     : 'gcc',
   'MacroModule'  : 'PartialMap',
   'OutputModule' : 'pjrcUSB',
   'DebugModule'  : 'full',
}

# main
parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output_dir", default='.',               help="(defualt='./') directory to store output")
parser.add_argument("-k", "--keyboards",  required=True,             help="space separated list of the keyboards to compile for, choose any of [ergodox-left, ergodox-right]")
parser.add_argument("-c", "--cmake_dir",  default='../controller',   help="(default='../controller') directory location of controller cmake")
parser.add_argument("-i", "--kll_dir",    default='../kll',          help="(default='../kll') directory look for KLL files")
parser.add_argument("-l", "--layers",     required=True,             help="semi-colon separated list of kll layers in order, assuming first layer is default map")
args = parser.parse_args()

# expand directories to be absolute paths
args.output_dir = os.path.abspath(args.output_dir)
args.kll_dir = os.path.abspath(args.kll_dir)
args.cmake_dir = os.path.abspath(args.cmake_dir)

keyboards_to_build = args.keyboards.split(' ')
for k in keyboards_to_build:
   if k not in keyboards:
      print('ERROR: keyboard {name} not a valid option ({valid_boards})'.format(name=args.keyboard, valid_boards=str(keyboards.keys())))
      continue

   # setup cmake options
   cmake_opts = default_cmake_opts.copy()
   cmake_opts.update(keyboards[k])

   # setup DefaultMap from first layer
   layers = args.layers.split(';')
   if not len(layers):
      print('ERROR: at least one layer must be specified')
      exit(1)

   cmake_opts['DefaultMap'] = '{layer}'.format(layer=layers.pop(0))
   if len(layers):
      partial_maps = ''
      for layer in layers:
         partial_maps += '{name}; '.format(name=layer)
      cmake_opts['PartialMaps'] = partial_maps

   ret_code = build(k, cmake_opts, args)
   if not ret_code:
      break;

