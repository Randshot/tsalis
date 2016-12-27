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
### Keyboard commands
|Key            |Action                                                 |
|---------------|------------------------------------------------------:|
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
|zx             |Decrease/increase steps per print                      |
|F1             |Open console                                           |
|1              |Step simulation one time                               |
|Space          |Run/pause simulation                                   |