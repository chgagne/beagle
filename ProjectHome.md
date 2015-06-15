# Open BEAGLE #

Open BEAGLE is a C++ Evolutionary Computation (EC) framework. It provides an high-level software environment to do any kind of EC, with support for tree-based genetic programming; bit string, integer-valued vector, and real-valued vector genetic algorithms; and evolution strategy.

The Open BEAGLE architecture follows strong principles of object oriented programming, where abstractions are represented by loosely coupled objects and where it is common and easy to reuse code. Open BEAGLE is designed to provide an EC environment that is generic, user friendly, portable, efficient, robust, elegant, and free.

## Features ##

### General features ###
  * Structured object oriented architecture
  * Smart pointers for automatic memory allocation management
  * Abstract factory, for runtime allocation of objects of any types
  * Parameters and algorithms dynamically configurable by files
  * Sophisticated logging mechanism with output in XML
  * Checkpoint mechanism for evolution recovery and results analysis
  * Population composed of multiple demes
  * History of best-of-run individuals for the whole population and for each demes
  * Population seeding from file
  * Master-slave model for parallel fitness evaluation

### Specific representations and algorithms ###
  * Bit string representation with decoder functions (binary and Gray-coded)
  * Integer-valued vector representation
  * Indices permutation vector representation
  * Real-valued vector GA representation
  * Anisotropic Self-Adaptive Evolution strategy (SA-ES)
  * Covariance Matrix Adaptation ES (CMA-ES)
  * Multiobjective optimization (including NSGA-II)
  * Complete implementation of tree-based GP
  * Automatically defined functions (ADF) for GP
  * Constrained GP tree operators with support for strongly-typed GP
  * Co-evolution support based on multi-threading
  * Many examples for the various algorithms

## Download ##

The current stable version is [3.0.3](http://code.google.com/p/beagle/downloads/detail?name=beagle-3.0.3.tar.gz), released on November 29, 2007. See the [Open BEAGLE Compilation HOWTO](http://code.google.com/p/beagle/downloads/detail?name=beagle-compil-howto-3.0.1-r1.pdf) for building instructions.

The current development version is [4.0.0-alpha2](http://code.google.com/p/beagle/downloads/detail?name=OpenBEAGLE-4.0.0-alpha2-Source.tar.gz), released on February 18, 2010.

With version 4.x, build system has been changed for [CMake](http://www.cmake.org), which provide a more elegant and portable building environment than the autotools. You also need to install [PACC version 1.4.1](http://code.google.com/p/beagle/downloads/detail?name=pacc-1.4.1-Source.tar.gz) beforehand, as it is no more included with the source distribution.

## Documentation ##

  * Overview paper [Genericity in Evolutionary Computation Software Tools: Principles and Case Study](http://vision.gel.ulaval.ca/en/publications/Id_605/PublDetails.php) is a good introduction paper on generic EC software in general, and on Open BEAGLE in particular.
  * The [Wiki](http://code.google.com/p/beagle/w/list) includes a variety a documents on the framework. See the [Documentation Overview](DocOverview.md) page for more details.
  * [Open BEAGLE Reference Manual](http://beagle.gel.ulaval.ca/refmanual/current) is providing specific information on the code.

## Mailing lists ##

The [Open BEAGLE Users List](https://groups.google.com/forum/#!forum/openbeagle-users) is intended to announce new releases, to ask questions of the framework, and to discuss on the future developments. It is highly suggested to join it, or at least to consult it frequently, if you are interested by the framework. Moreover, if you have problems with the framework, first consult this list to see if any previous discussions can help you, then ask a question on the users list if you still cannot solve it.

The mailing list on [Yahoogroups](http://tech.groups.yahoo.com/group/openbeagle) had been replaced by the [users list on Google Groups](https://groups.google.com/forum/#!forum/openbeagle-users), and so is no more in use. However, it consists in a very complete knowledge base that you can consult.

The [Open BEAGLE Developers List](https://groups.google.com/forum/#!forum/openbeagle-developers) is intended to users interested in contributing to the framework developments. It is more technically oriented and generally has a higher traffic than the users list, with all commits and issues. It also presents the new features added to the bleeding edge version of the framework, available in the Mercurial repository.

## Other libraries ##

  * [DEAP](http://code.google.com/p/deap): an evolutionary computation framework in Python, for rapid prototyping and testing of ideas. Done by the same team than Open BEAGLE!
  * [BEAGLE Puppy](Puppy.md): a minimalist GP library in C++, based on the Open BEAGLE architecture.
  * [ECJ](http://cs.gmu.edu/~eclab/projects/ecj/): probably the most used generic EC framework in Java.
  * [Evolving Objects](http://eodev.sourceforge.net/): another widely used EC framework in C++, but design with a different design philosophy.
  * [GPLab](http://gplab.sourceforge.net/): for those who want to do GP in Matlab.

## Contribute ##

This is free software which we are proud to provide to the community. If you are using Open BEAGLE in your work or studies, and you are gaining some knowledge of it, please consider making your share for improving it. We are always open to get your contribution on the code or the documentation.

## Cite ##

If you are publishing scientific papers on work using Open BEAGLE please cite the following reference.

Christian Gagné and Marc Parizeau, [Genericity in Evolutionary Computation Software Tools: Principles and Case Study](http://vision.gel.ulaval.ca/en/publications/Id_605/PublDetails.php), _International Journal on Artificial Intelligence Tools_, vol. 15, num. 2, p. 173-194, 2006.
```xml

@article{Gagne2006,
author    = { Christian Gagn\'e and Marc Parizeau },
title     = { Genericity in Evolutionary Computation Software Tools: Principles and Case Study },
volume    = { 15 },
number    = { 2 },
pages     = { 173-194 },
year      = { 2006 },
journal   = { International Journal on Artificial Intelligence Tools },
}```

## Copyright ##

Copyright © 1999-2012 by [Christian Gagné](http://vision.gel.ulaval.ca/~cgagne) and [Marc Parizeau](http://vision.gel.ulaval.ca/en/people/Id_17/index.php), with the contributions of many collaborators.

This library is free software; you can redistribute it and/or modify it under the terms of the version 3 of the [GNU Lesser General Public License](http://www.gnu.org/licenses/lgpl.html) as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.