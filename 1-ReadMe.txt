+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Open BEAGLE

Copyright (C) 1999-2009
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


ABOUT
=====

Open BEAGLE is a versatile C++ evolutionary computation framework.
It provides a software environment to do any kind of EC, with a
fully-featured support for genetic programming and vector-based
evolutionary algorithms such genetic algorithms and evolution
strategies.


INSTRUCTIONS TO BUILD THE FRAMEWORK
===================================

New build system of Open BEAGLE is based on CMake. For building it,
follow usual CMake method by running CMake command-line or GUI using
as source directory the main directory of the archive. CMake will 
generate the appropriate build system according to the generator 
selected.

There are several important CMake variables that you may which
to set. The most important being:

- PACC_DIR: Setting path where an installation of PACC can be
  found (default: /usr/local).
- CMAKE_BUILD_TYPE: Type of build made, with possible values being
  Debug or Release (default: Debug).
- CMAKE_INSTALL_PREFIX: Where the library is installed
  (default: /usr/local).


GETTING STARTED
===============

You can experiment with the different examples provided in folder
'examples'. The ReadMe.txt files of these examples explain
how to build them, The usage of the example binaries is provided by
executing them with the command-line argument '-OBusage'.
Detailed help is also provided with argument '-OBhelp'.


DOCUMENTATIONS
==============

Consult the Open BEAGLE web page (http://beagle.gel.ulaval.ca)
to get all the necessary documents to use this framework.

