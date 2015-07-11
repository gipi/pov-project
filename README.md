If you want to try this project you have to initialize using repo
the dependencies

```
$ mkdir pov && cd pov
$ repo init -u http://github.com/gipi/pov-project.git -b master
$ repo sync
```

Now you have some directory:

 - ``src`` containing the source code
 - ``build`` containing the library and build system for AVR

or you can download seperately the build system (my ``avr_stuffs`` repo)
and use the ``CORE_BUILD_DIR`` variable pointing to it

    $ make -C firmware/ CORE_BUILD_DIR=/path/to/build/system
