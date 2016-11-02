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
 
def build(name, opts):
   print('\nBuilding {name}...'.format(name=name))
   # create and cd to output directory
   os.chdir(args.output_dir)
   if os.path.exists(name):
      shutil.rmtree(name)
   os.makedirs(name, exist_ok=True)
   os.chdir(name)

   # copy KLL files to output dir
   for filename in glob.glob('{kll_dir}/*'.format(kll_dir=args.kll_dir)):
      shutil.copy(filename, './')
   
   cmd = generate_cmake_cmd(args.cmake_dir, opts)
   print(cmd)
   exit_code = subprocess.call(cmd, shell=True) 
   if exit_code:
      print('ERROR: failed to cmake {name}'.format(name=name)) 
   
   exit_code = subprocess.call("make")
   if exit_code:
      print('ERROR: failed to make {name}'.format(name=name)) 
   

# keyboard specific cmake options  
keyboards = {
   'ergodox' : {
      'CHIP' : 'mk20dx256vlh7',
      'ScanModule' : 'Infinity_Ergodox'
      }
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
parser.add_argument("-v", "--verbose",                               help="increase output verbosity")
parser.add_argument("-o", "--output_dir", default='.',               help="(defualt='./') directory to store output")
parser.add_argument("-k", "--keyboard",   default="ergodox",         help="(default='ergodox') space separated list of kll layers in order")
parser.add_argument("-c", "--cmake_dir",  default='../controller',   help="directory location of controller cmake")
parser.add_argument("-i", "--kll_dir",    default='../kll',          help="directory look for KLL files")
parser.add_argument("-l", "--layers",     required=True,             help="space separated list of kll layers in order")
args = parser.parse_args()

# expand output directory to be absolute path
args.output_dir = os.path.abspath(args.output_dir)
args.kll_dir = os.path.abspath(args.kll_dir)
args.cmake_dir = os.path.abspath(args.cmake_dir)

# add keyboard specific options
if args.keyboard not in keyboards:
   print('ERROR: keyboard {name} not a valid option ({valid_boards})'.format(name=args.keyboard, valid_boards=str(keyboards.keys())))
   exit(1)

default_cmake_opts.update(keyboards[args.keyboard])

# setup DefaultMap from first layer
layers = args.layers.split(' ')
if not len(layers):
   print('ERROR: at least one layer must be specified')
   exit(1)
 
default_cmake_opts['DefaultMap'] = '{layer} lcdFuncMap'.format(layer=layers.pop(0))
if len(layers):
   partial_maps = ''
   for layer in layers:
      #partial_maps += '{name} lcdFuncMap; '.format(name=layer) 
      partial_maps += '{name}; '.format(name=layer) 
   default_cmake_opts['PartialMaps'] = partial_maps

# setup left hand cmake options
left_cmake_opts = default_cmake_opts.copy()
left_cmake_opts['BaseMap'] = 'scancode_map leftHand slave1 rightHand'

# setup right hand cmake options
right_cmake_opts = default_cmake_opts.copy()
right_cmake_opts['BaseMap'] = 'scancode_map rightHand slave1 leftHand'

build('left', left_cmake_opts)
build('right', right_cmake_opts)


