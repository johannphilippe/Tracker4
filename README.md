# Tracker 4 

In development version of new jo_tracker. The new name is not found yet. 

In order to build this software, you need to set ELEMENTS_ROOT to [Elements](https://github.com/cycfi/elements/tree/artist_port) root directory.
You can refer to Elements build instructions.
```
mkdir build && cd build
cmake .. -DELEMENTS_ROOT="/home/root/to/elements"
cmake --build .
```


## Libraries

This repository includes some open-source libraries source code : 

* [Nlohmann::json](https://github.com/nlohmann/json) - MIT License
* [LexerTK](https://github.com/ArashPartow/lexertk) - the version included is modified. - MIT License
* [Portable File Dialog](https://github.com/samhocevar/portable-file-dialogs) - DWTYWTPL


## Dependencies

This software requires a few libraries in order to build : 

* [Elements](https://github.com/cycfi/elements/tree/artist_port) - artist_port branch
* [Luajit](https://luajit.org/) or [Terra](https://terralang.org/)
