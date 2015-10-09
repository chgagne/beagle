+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Function maximization (maxfct): A simple GA example with Open BEAGLE

Copyright (C) 2001-2006
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'maxfct'. Usage options is described by
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

  Bit strings of 125 bits, constructed from the function's 5 arguments $x_i$,
  each encoded with 25 bits on the interval $[-200,200]$.

Fitness
=======

  Value of $f(x_1,x_2,x_3,x_4,x_5)$.

