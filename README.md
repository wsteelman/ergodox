# ergodox

This is a repo to track my custom KLL layouts for the Ergodox Infinity.  

Included in the repo is:
  kll/        custom KLL files
  build/      python build script
  lcd/        bmp images used for custom lcd output
  gui_layout  old KLL and JSON from the I:C web configurator
  
  
## Build
Most notable for public consumption is the build script.  I found the provided BASH build scripts to be unsuitable for my use. The built-in scripts assume that the custom KLL must already exist in the build path and it assumes that the root cmake file is in (../..).  So in general it doesn't handle things well when KLL, cmake, and build directories are in different places.  The script here has the following usage, which allows for KLL, cmake, and output (build) directories to be disparate:

usage: build.py [-h] [-o OUTPUT_DIR] -k KEYBOARDS [-c CMAKE_DIR] [-i KLL_DIR]
                -l LAYERS

optional arguments:
  -h, --help            show this help message and exit
  -o OUTPUT_DIR, --output_dir OUTPUT_DIR
                        (defualt='./') directory to store output
  -k KEYBOARDS, --keyboards KEYBOARDS
                        space separated list of the keyboards to compile for,
                        choose any of [ergodox-left, ergodox-right]
  -c CMAKE_DIR, --cmake_dir CMAKE_DIR
                        (default='../controller') directory location of
                        controller cmake
  -i KLL_DIR, --kll_dir KLL_DIR
                        (default='../kll') directory look for KLL files
  -l LAYERS, --layers LAYERS
                        semi-colon separated list of kll layers in order,
                        assuming first layer is default map
 
The script will then perform the following actions
1. The OUTPUT_DIR will be create, if it doesn't already exist
2. All existing files in the OUTPUT_DIR directory will be deleted in preparation for a new build.  
3. All KLL files from the KLL_DIR will be copied into the OUTPUT_DIR.
4. The cmake command line will be created and run
5. If cmake was successfull, make will be run
6. The output of make will be found in OUTPUT_DIR/KEYBOARD

Future changes
- Add other keyboards.  This can be easily done by adding the appropriate build options to the 'keyboards' map in the script.
- Update the script so that keyboard specific data (from the 'keyboards' map) is stored in external config files (i.e. yaml)
