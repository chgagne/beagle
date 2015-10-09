+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

SPAM e-mail database (spambase):
Machine learning using strongly-typed GP

Copyright (C) 2001-2007
by Christian Gagne <cgagne@gmail.com>
and Marc Parizeau <parizeau@gel.ulaval.ca>

+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


Getting started
===============

  Example is compiled in binary 'spambase'. Usage options is described by
  executing it with command-line argument '-OBusage'. The detailed help can
  also be obtained with argument '-OBhelp'.

Objective
=========

  Find a program the will successfully predict whether a given e-mail is spam
  or not from some extracted features.

Comments
========

  The evolved programs works on floating-point values AND Booleans values.
  The programs must return a Boolean value which must be true if e-mail is
  spam, and false otherwise.

  Don't expect too much from this program as it is quite basic and not oriented
  toward performance. It is given to illustrate the use of strongly-typed GP
  with Open BEAGLE. From a machine learning perspective, it is mainly wrong.

Terminal set
============

 IN0, IN1, ...  up to IN56, the e-mail features.      [Double]
 0 and 1, two Boolean constants.                      [Bool]
 Ephemeral constants randomly generated in $[0,100]$  [Double]

Function set
============

 AND               [Inputs: Bool,   Output: Bool]
 OR                [Input:  Bool,   Output: Bool]
 NOT               [Inputs: Bool,   Output: Bool]
 +                 [Inputs: Double, Output: Double]
 -                 [Inputs: Double, Output: Double]
 *                 [Inputs: Double, Output: Double]
 /                 [Inputs: Double, Output: Double]
 <                 [Inputs: Double, Output: Bool]
 ==                [Inputs: Double, Output: Bool]
 if-then-else      [1st Input: Bool, 2nd & 3rd Input: Double, Output: Double]

Fitness cases
=============

  A random sample of 400 e-mails over the database, re-chosen for
  each fitness evaluation.

Fitness
=======

  Classification rate over the 400 e-mails sampled.

Stopping criteria
=================

  When an individual get a 100% classification rate (fitness of 1.) or when
  the evolution reaches the maximum number of generations.

Reference
=========

  Machine learning repository,
  http://www.ics.uci.edu/~mlearn/MLRepository.html
