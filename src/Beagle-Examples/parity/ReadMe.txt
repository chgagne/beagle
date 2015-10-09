+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Even-6 parity (parity): A GP example using ADFs with Open BEAGLE

Copyright (C) 2001-2007
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'parity'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a program that produces the value of the Boolean even-6 parity given
  six independent Boolean inputs.

Terminal set
============

  IN0, IN1, IN2, IN3, IN4, and IN5, the inputs of the function.
  0 and 1, two Boolean constants.

Function set
============

  AND
  OR
  NAND
  NOR

Automatically defined functions
===============================

  Two ADFs used, each with two arguments inputs.

Fitness cases
=============

  All the $2^6 = 64$ combinations of the six input Booleans.

Fitness
=======

  Rate of correct outputs obtained over all the 64 fitness cases.

Stopping criteria
=================

  When the evolution reaches the maximum number of generations.

Reference
=========

  John R. Koza, "Genetic Programming II: Automatic Discovery of Reusable
  Programs", MIT Press, 1994, pages 157-199.
