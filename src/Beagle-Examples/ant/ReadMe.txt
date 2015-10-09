+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Artificial ant (ant)

Copyright (C) 2006 by Christian Gagne <cgagne@gmail.com>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'ant'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a program that produces to control an artifical ant so that it can
  eat all the food located in a given environement.

Terminal set
============

  L: Turn ant 90 degrees counter-clockwize
  R: Turn ant 90 degrees clockwize
  M: Move ant one case front

Function set
============

  IFA: If there is food sensed on front case execute second argument, else
       execute third argument
  P2: PROGN2 lisp function, execute first argument then second argument.
  P3: PROGN3 lisp function, execute first, second, and then third arguments.

Fitness
=======

  Number of pieces of food picked up before the ant times out with 400
  operations.

Stopping criteria
=================

  When the evolution reaches the maximum number of generations or an ant
  picked up all the 89 pieces of food in one evaluation.

Reference
=========

  John R. Koza, "Genetic Programming I: On the Programming of Computers by Means
  of Natural Selection", MIT Press, 1992, pages 147-161.
