+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Multiobjective 0/1 Knapsack (knapsack): Multiobjective GA example

Copyright (C) 2003-2007
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'knapsack'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a combination of objects to put in a knapsack with a minimum weight but
  a maximum value.

Representation
==============

  Bit strings made of 24 bits, where the ith bit designate whether the ith
  object is put in the bag or not. The value and weight of each objects
  is randomly generated at each runs.

Fitness
=======

  First objective is the maximization of the knapsack's objects value. The
  second objective is the maximization of  (maximum knapsack weight -
  objects weight), that is the minimization of the weight.

