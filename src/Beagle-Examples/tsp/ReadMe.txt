+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Traveling Salesman Problem (tsp): Indices integer vector GA example

Copyright (C) 2005-2006 by Christian Gagne <cgagne@gmail.com>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'tsp'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a the shortest path to visit all the nodes of a randomly generated
  graph.

Representation
==============

  Order in which the nodes of the graph are visited. The integers in the
  vector represent the indices of the nodes to visit.

Fitness
=======

  Sum of the inter-node distance given the path represented by an integer
  vector.

