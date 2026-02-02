# ZBilliards

## SFML (C++) billiards game

<img width="2758" height="1566" alt="image" src="https://github.com/user-attachments/assets/3aa9f458-98ec-49f0-a364-ed5fe10eb101" />

### ABOUT THE PROJECT
This game is one of many projects that I undertook in the early days of my programming self-teaching. The projects of this period were motivated primarily by the desire to know if I could "solve" the challenge of bringing this or that mechanic or concept into being through programming language, largely without consulting any other sources to see how it was done. The projects were characterized by a lack of concern for code organization, best practices, readability, or maintainability, because my free time was very limited and my chief objective was getting the satisfaction of knowing I got the idea to work. Most of the time, when I achieved that much of the goal, I was ready to move on to the next logical challenge rather than pore over the details of making a well-rounded product fit for public consumption. 
  
I've taken a little time to restructure some things before posting the code here; nonetheless, much about the program remains primitive or suboptimal as it was originally written. Most notably, I hardcoded my graphics to the size of the screen I worked on at the time, and have had to write some extra code to make them display acceptably on different screen sizes. 

### FILE DESCRIPTIONS
* **sfmlApp:**  Implements `main()` and the abstract app
* **billiards:**  Implements primary graphical elements; physics; game logic
* **objects:**  Small structs representing balls, pockets, etc.
(From my "reusable modules" repo: github.com/johnnywz00/SFML-shared-headers)
* **jwz:**  C++ utility functions, #defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific

### BUILDING INSTRUCTIONS
Ready-made program files are available on the Releases page of this repository, with versions for MacOS, Windows, and Linux. If you have reasons for building from source:
- Clone this repository, and navigate to the root folder of that clone in a terminal window.
- Run:
   cmake -B build
   cmake --build build
