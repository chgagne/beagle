+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Boolean 11-multiplexer (multiplexer)

Copyright (C) 2006 by Christian Gagne <cgagne@gmail.com>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'multiplexer'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a program that produces the value of the Boolean 11-multiplexer, that
  is to return the value of the input line between the 8 input channels
  D0-D7 according to the address given by inputs A0-A2.

Terminal set
============

  D0, D1, D2, D3, D4, D5, D6, D7: the input channels.
  A0, A1, A2: the input addresses.

Function set
============

  AND
  OR
  NOT
  IF      (3-arguments boolean if-then-else)

Fitness cases
=============

  All the $2^8 = 2048$ test cases possible are used for fitness evaluation.

Fitness
=======

  Rate of correct outputs obtained over all the 2048 fitness cases.

Stopping criteria
=================

  When the evolution reaches the maximum number of generations.

Reference
=========

  John R. Koza, "Genetic Programming I: On the Programming of Computers by Means
  of Natural Selection", MIT Press, 1992, pages 170-187.
