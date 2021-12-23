# Track

- Add a longer text box at top of trackset that can be editable (linked to the selected cell) or enslaved. Must be able to "deactivate" (sniper track). 
- tracker_track keys : (backspace, tab etc...). 
- For cells : separate focus from select (can be selected, but not focused to allow other manipulations - like select multiple cells )
- Color modes for lines : antescofo/chuck, lua, Csound score, realtime line selection (csound perf).
- simplify architecture (do the view.post in top architecture so there is no need to post/refresh a lot of events)
- Callback for when a new track is selected (unselect current track)
- But do not unselect if the upper text box is selected (or anything else).
- If I move with arrows, does not enter edit mode. If I double click or type enter or characters, it enters edit mode and types into the cell. If I escape once it exits edit mode, twice exits track focus.
- Reduce P2 size (max 2 or 3 characters)
- Reduce others as well ?

