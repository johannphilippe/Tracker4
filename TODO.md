# TODO 

Todo list for this new version of jo_tracker created with elements.

The MUST have of this, is that we need to be able to process everything in Csound (export a CSD, render audio etc from Csound)

##  General

* Find a new name

### Build & delivery

* Shipped with luajit
* Shipped with csoundfaust opcodes
* Shipped with my opcodes -> Lua etc
* Shipped with UDOs

The project must contain or import those sources and build it so it can be installed with the tracker.

## GUI

### Standard elements

* Improve spinbox allowing to type text inside
* Scroller in sync with another one 

### Tracker specific elements

* Code editor
* Curve editor -> curve comparisons
* Track cell -> scroll to increment
* Tracker cell -> placeholder or tooltip for p-field name
* Tracker track -> a way to keep the action bars in the view
* Sniper track -> add a list of vertical automations (showable, hidable)
* Sniper track -> event mode selection
* Sniper track -> text editor


### Audio elements

* Vue meter
* FFT Scope (realtime frequency in x axis, amplitude in y)
* Oscilloscope (improve)
* FFT History scope (Time in x axis, frequency in y)

## Functionnalities 

### To Add 

* Action list for common actions, and user defined actions (API)
* Different events types -> csound score, chuck, lua, soundfile
* Render dialog
* Event types -> Csound score, Chuck process, luajit process, Faust, Waveform, and one day Antescofo process

### To improve 

* Theme -> make every component dependant on theme
* Theme -> allow building theme from json
* Theme menu

### Csound

* In csound automatic code generation, add macros to instrument numbers `#define oscili=0'


### Events 

* Faust : All this can be csound code generation
    - In tracker track -> a faust code, and a few parameters (called p-fields) (why not curves)
    - In sniper track -> faust code, and automations of sniper track
* Chuck : This will require to either build a csound opcode to run it, either have to pre-run events when exporting CSD (can be real time as well)
    - In tracker track -> a chuck code, designed to generate events, and parameters (automations ?)
    - In sniper track -> Same, with automations
* Csound event 
    - In tracker track - csound i statement, with some homemade macros (or Csound ones ?, for example the scale macro)
    - In sniper track, same (automations might be tough, except if it has )
* Luajit Process same as chuck, + needs realtime implementation
* Waveform : Csound code formatting. Requires drag & drop to be implemented though. 
* Why not JSFX ? Already there as csound opcode

## Bugs

* Tempo track : keys do not refresh the view
* Basic_input_box placeholder not in right place (main text box, and options)
* Improve drag event in sniper track -> drag from the middle must not be an obligation, create an event and expand it immediatly