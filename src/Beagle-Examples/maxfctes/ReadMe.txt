+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Function maximization with Evolution Strategy (maxfctes):
An ES example

Copyright (C) 2001-2007
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'maxfctes'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find the maximum of the following 5D function (LaTeX syntax):
  $f(x) = \frac{161.8}{(u_N^2 \sum_{k=0}^{N-1}(x_k^2 + u_k^2))}$
  with $x = <x_0, x_1, ..., x_{N-1}>$, $u_{k+1} = x_k + u_k$, $x_k$
  in $[-200,200]$ for all $k$, $N = 5$ and $u_0 = 10$.

Representation
==============

  Vector of five ES pairs, each pair being an association of a value
  and a mutation strategy parameter. The values represents the function's
  five arguments \f$x_i\f$. They are limited in the interval
  \f$[-200,200]\f$ at the evaluation time.

Fitness
=======

  Value of $f(x_1,x_2,x_3,x_4,x_5)$.
