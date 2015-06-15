BEAGLE Puppy is a C++ library for Genetic Programming (GP). It is a stripped-down version of the [Open BEAGLE](http://code.google.com/p/beagle/) Evolutionary Computations (EC) framework.

Open BEAGLE is a versatile generic object oriented environment for EC that supports a lot of nice features. Every part of it can be replaced dynamically, without modifications in the core libraries. In that sense it is very useful for graduate students and researchers that are experimenting with EC and are developing new algorithms and methods. But this genericity and versatility come with the cost of an important base code and some training efforts needed for mastering it.

BEAGLE Puppy represents an exercise of minimalism, where the core GP algorithms of Open BEAGLE are repackaged into the simplest C++ form possible. The result is a light library of about 1600 lines of code, including the comments. In that sense, it represents a easier introduction to GP in C++. It is also an excellent stepping-stone to Open BEAGLE as the wording and usage patterns are very close.

As with Open BEAGLE, BEAGLE Puppy makes heavy use of STL containers and generic algorithms. It uses a tree representation packed into array with nodes in the prefix ordering. This complicates a little the code for the crossover and mutation operations, as the tree needs to be unpacked for these operation. But it allows lightning-fast programs interpretation and avoid significant memory scrambling that happens with the traditional pointer-based implementation.

BEAGLE Puppy supports tree-based GP with sub-tree crossover, standard (Koza's) and swap-point mutation, tournament and roulette selection. Primitives (terminals and functions) are defined by sub-classing an abstract class. Ephemeral random constants are supported. No support for esoteric crossover and mutation, ADFs, multi-tree individuals, strongly-typed GP, multi-objective optimization, co-evolution, XML file formats, and other EC representation are planned. If that interest you, Open BEAGLE is probably more suited for you.

## Documentation ##

A [Reference Manual](http://code.google.com/p/beagle/downloads/detail?name=puppy-refmanual-0.1.tar.gz) generated from the source code is available.

Another good starting point is to take a look into the code. As it is made to be minimal and simple, this is probably the best way to master the library.

[Open BEAGLE Users List](https://groups.google.com/forum/#!forum/openbeagle-users) is intended to announce new Open BEAGLE releases, to ask questions of the framework, and to discuss on the future developments. Announcements related to its little brother, BEAGLE Puppy will also be made through this list.

## Download ##

The current stable version is [0.1](http://code.google.com/p/beagle/downloads/detail?name=puppy-0.1.tar.gz) (release on February 11, 2004).

## Building the library ##

### Building the library on Unix ###

To build BEAGLE Puppy on Unix, uncompress the distribution, open a shell, `cd` to the uncompressed distribution main directory, and type the following commands.

```
./configure
make clean
make
```

This will configure and compile the library and examples. Optionally, you can install the library and examples binary by typing the following command.

```
make install
```

By default, the compiled libraries are installed in folder `/usr/local/lib`, while the headers are installed in folder `/usr/local/include`. You can specify the installation folder by giving the option `--prefix=PATH` to the 'configure' script.

`configure` recognizes the following options to control how it operates.

  * `--prefix=PATH`: Set the main installation folder (default: `/usr/local`).
  * `--disable-shared`: Disable the build of the shared framework libraries.
  * `--disable-static`: Disable the build of the static framework libraries.
  * `--cache-file=FILE`: Use and save the test results in file `FILE` instead of `./config.cache`. Set `FILE` to `/dev/null` to disable caching, for debugging `configure`.
  * `--help`: Print a summary of the options to `configure`, and exit.
  * `--quiet`, `--silent`, `-q`: Do not print messages saying which checks are being made. To suppress all normal output, redirect it to `/dev/null` (any error messages will still be shown).
  * `--version`: Print the version of autoconf used to generate the `configure` script, and exit.
`configure` also accepts some other, not widely useful, options.

### Building the library on MS Visual C++ .Net 2003 ###

Open the solution file `puppy.sln` in folder `MSVCPP`, then compile either a debug or a release version of the library and the examples provided.

## Copyright ##

BEAGLE Puppy: a minimalist genetic programming library in C++.

Copyright © 1999-2004 by [Christian Gagné](http://vision.gel.ulaval.ca/~cgagne), under the direction of [Marc Parizeau](http://vision.gel.ulaval.ca/en/people/Id_17/index.php).

This library is free software; you can redistribute it and/or modify it under the terms of the [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html) as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

This project is done in the context of a research project at the [Computer Vision and Systems Laboratory](http://vision.gel.ulaval.ca) of [Université Laval](http://www.ulaval.ca).