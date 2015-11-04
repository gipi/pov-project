This directory contains tools used during development.

## ``analyze``

This application prints in the ``stdout`` the values of a
dumped eeprom.

Below a sample of its usage ([gnuplot](http://www.gnuplot.info/) invocation inspired by this [answer](http://stackoverflow.com/questions/17543386/pipe-plot-data-to-gnuplot-script)):

```
$ ./tools/analyze tools/pov_dumped.eep | (cat > /dev/shm/mytempfile && trap 'rm /dev/shm/mytempfile' EXIT && gnuplot -e "set terminal dumb; plot '/dev/shm/mytempfile' using 1 with lines")


  10 ++----------+----------+-----------+-----------+---*------+----------++
     +           +          +         '/dev/shm/mytempfile' using 1 ****** +
     |                                              *    *                 |
   5 ++                     **   *   *   **   **   **    *                ++
     |   *   *  **  **  **  **  **   **  ***  **   *     *                 |
     **  *  **  **  **  **  **  **   **  * *  * *  *     *    *            |
   0 **  ** **  **  **  **  **  *** * *  * *  * *  *     *** **           ++
     **  ** **  **  ** * *  **  * * * *  * *  * *  *     *** **            |
     |* *** **  ** *** * * ***  * * * *  * * ** * **     *** **  *         |
  -5 +* * * * * ** * * * * *  * * * * * ** * *  * *      *** ** **        ++
     |* * * * * ** * * * * *  **  * *  **  * *  * *      *** * **          |
     |*** * * ** *** *** ***  **  * *  **  * *  * *      *** * **          |
     | ** * * **  **  **  **  **  * *  **   **   **      *** * **          |
 -10 ++** * * **  **  **  **  **  * *  **   **   **      *** * **         ++
     | ** **  **  **  **  **  **  **   **   **   **      * * * **          |
     | ** **  **  **  **  **  **   *   **   **   **      *  ** **          |
 -15 ++** **  **  **  **  **  **   *    *                *  ** **         ++
     | **  *  **  *                                      *  *  **          |
     + *         +          +           +           +    *  *  **          +
 -20 ++----------+----------+-----------+-----------+----------+----------++
     0          200        400         600         800        1000        1200

```

## ``min_max``

This application is mainly for testing purpose: using an output from ``analyze``
logs the state changes of the ``struct state_t``.

```
$ ./tools/analyze tools/pov_dumped.eep | ./tools/min_max
-1      -1      0       0
0       +0      1       0
0       +0      1       0
0       +0      1       0
1       +1      1       0
2       +2      1       0
2       +2      1       0
2       +2      1       0
2       +2      1       0
1       +1      2       2
1       +1      2       0
0       +0      2       0
0       +0      2       0
0       +0      2       0
-1      -1      2       0
-2      -2      2       0
-3      -3      2       0
-5      -5      2       0
-6      -6      2       0
-7      -7      2       0
-8      -8      2       0
-9      -9      2       0
-17     -17     2       0
-18     -18     2       0
-18     -18     2       0
-18     -18     2       0
-18     -18     2       0
-17     -17     1       1
-17     -17     1       0
-17     -17     1       0
-16     -16     1       0
-16     -16     1       0
-15     -15     1       0
-14     -14     1       0
-13     -13     1       0
```

