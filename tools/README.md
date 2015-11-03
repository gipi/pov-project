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
