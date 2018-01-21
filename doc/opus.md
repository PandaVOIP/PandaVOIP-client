# Opus library

The project uses the Opus codec for voice encoding and decoding. We use the latest version (at time of writing) which is 1.2.1. Its home is: http://opus-codec.org

## Compilation and installation

### Windows

On windows, the project uses MinGW. Opus is compiled from source using the msys utility under MinGW.

Make sure MinGW is in your path and `gcc` and `make` are both found by the command line.

1. Download the libopus source from http://opus-codec.org/release/stable/2017/06/26/libopus-1_2_1.html
2. Extract this to your msys home which should look like `C:\MinGW\msys\1.0\home\USERNAME\`
3. Open the MSYS terminal and navigate to the source.
4. `./configure --prefix=/home/USERNAME/opusbuild/`
5. `make -j4`
6. `make install`
7. Now copy all of the files from the `opusbuild` directory into their respective folders in `C:\MinGw\`.

This should be all you need to compile and install the libopus files.