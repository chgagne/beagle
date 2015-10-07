+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Symbolic regression (symbreg): A simple GP example with Open BEAGLE

Copyright (C) 2001-2006
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'symbreg'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a function of one independent variable and one dependent variable, in
  symbolic form, that fits a given sample of 20 $(x_i,y_i)$ data points,
  where the target function is the quadratic polynomial $x^4 + x^3 + x^2 + x$.

Terminal set
============

  X (the independent variable)
  PI
  Ephemeral constants randomly generated in $[-1,1]$

Function set
============

  +
  -
  *
  /     (protected division)
  SIN
  COS
  EXP
  LOG   (protected logarithm)

Fitness cases
=============

  The given sample of 20 data points $(x_i,y_i)$, randomly chosen within
  interval [-1,1].

Fitness
=======

  $\frac{1.}{1.+RMSE}$ where RMSE is the Root Mean Square Error on the
  fitness cases.

Stopping criteria
=================

  When the evolution reaches the maximum number of generations.

Reference
=========

  John R. Koza, "Genetic Programming: On the Programming of Computers by Means
  of Natural Selection", MIT Press, 1992, pages 162-169.

