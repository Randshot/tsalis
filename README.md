# TSALIS
*TSALIS* is a text user interface (TUI) designed to communicate with *SALIS*.
Its only dependencies are NCurses and the *SALIS* library itself. It should be
portable enough and should run easily in any terminal environment.

## Building instructions
You'll need nothing but a C compiler (C89). You must build the program and link
it with *SALIS* (e.g. *libsalis.a*) and NCurses. A sample makefile
(Sample.make) is provided for GNU Make. Feel free to edit the makefile as
needed. If you run into any difficulties, please let me know!

## List of commands
### Command-line arguments
You may run *TSALIS* from the terminal in any of these three ways (arguments
are being represented by *XX*). Note that, upon exit, *SALIS* automatically
generates a save file called *def.salis*. This save file may be freely renamed
and reloaded as needed.

|Arguments      |Action                                                                         |
|:--------------|------------------------------------------------------------------------------:|
|tsalis         |If file *def.salis* exists in directory, loads simulation from that file.      |
|tsalis         |If file *def.salis* does not exist, creates new simulation (memory size 2^16). |
|tsalis n*XX*   |Creates new simulation with memory size 2^*XX*.                                |
|tsalis l*XX*   |Loads simulation from file named *XX*.                                         |

### Keyboard commands
|Key            |Action                                                 |
|:--------------|------------------------------------------------------:|
|Enter          |Flip page                                              |
|Arrow keys     |Scroll page                                            |
|Backspace      |Scroll back to top/left                                |
|g              |Toggle gene/data view (in PROCESS page)                |
|ws             |Zoom (in WORLD page)                                   |
|WS             |Pan vertically (in WORLD page)                         |
|ad             |Pan horizontally (in WORLD view)                       |
|q              |Pan to 0 (in WORLD view)                               |
|k              |Pan to selected process (in WORLD view)                |
|op             |Select next/previous process                           |
|fl             |Select first/last process                              |
|nm             |Decrease/increase steps per print                      |
|c              |Open console (simulation must be paused)               |
|1              |Step simulation one time                               |
|Space          |Run/pause simulation                                   |
|X              |Save and quit simulation                               |

### Console commands
The console opens up when 'c' is pressed. Commands, with their respective
parameters separated by underscores, may be written in order to modify or
control some aspects of the simulation. Parameters here are represented by
*XX*.

|Command     |Param. 1    |Param. 2    |Action                                                           |
|:-----------|:-----------|:-----------|----------------------------------------------------------------:|
|i*XX*_*XX*  |address     |instructions|Writes given instructions into address.                          |
|c*XX*_*XX*  |address     |file name   |Compiles given file into address.                                |
|n*XX*_*XX*  |address     |size        |Initializes organism of given size into address.                 |
|k           |---         |---         |Kills organism at bottom of queue (first organism).              |
|s*XX*       |process id  |---         |Scroll (PROCESS page) to given process.                          |
|p*XX*       |process id  |---         |Select given process.                                            |
|ew*XX*      |rate        |---         |Set WRITE ERRORS to given rate.                                  |
|ec*XX*      |rate        |---         |Set COSMIC RAYS to given rate.                                   |
|ep*XX*      |rate        |---         |Set PROCESS DATA MUTATIONS to given rate.                        |
|bm*XX*      |size        |---         |Set min allowed memory block size to given value.                |
|bx*XX*      |size        |---         |Set max allowed memory block size to given value.                |
|h           |---         |---         |Toggle "halting" (if on, organisms halt on faulty instructions). |