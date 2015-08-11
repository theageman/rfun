# rfun
Having fun with resistors

This is a little project that i created to start
working with Qt5.5/QML/C++.

Although this is a learning project for me, it might
still be an interesting piece of software.

The basic idea is based on developing electronic 
circuits with resistors. Resistors can be used
in serial and parallel combinations.
My goal was to find an algorithm that automatically
finds an optimal combination of serial and parallel
circuits for any given number of resistors via 
recursion. Basically the algo tries out all possible
serial and parallel combinations, everytime calculating
the resulting error and storing the combination with
the smallest error.

A second algorithm is then responsible for creating
a bitmap of that solution showing a schematic.
