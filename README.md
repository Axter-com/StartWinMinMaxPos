# StartWinMinMaxPos
Start an executable minimize, maximize, and/or at a certain window position and size.

### Usage:
Use StartWinMinMaxPos to start up an executable minimize, maximize, and/or at a certain position and window size.

- To get it to work, copy StartWinMinMaxPos.exe to the same directory of the target executable.
- Rename the target executable by inserting ".Original" into the name.
  - Examples: 
    - "TheProgramName.Original.exe"
    - "NotePad.Original.exe"
- Rename StartWinMinMaxPos.exe to the target executable.
- By default, the target program will start minimized. To get different behavior, create an ini file that has the same name as the executable but with ".ini" appended to the name.
  - Example:
    - "NotePad.exe.ini"


See the StartWinMinMaxPos.exe.ini file for details on how to populate the INI file.
