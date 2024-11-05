# ZBilliards

## SFML (C++) billiards game

<img width="1119" alt="ZBillPrvw" src="https://github.com/user-attachments/assets/6ce7a902-e4e3-4e45-8f22-3744094a3869">

### ABOUT THE PROJECT
This game is one of many projects that I undertook in the early days of my programming self-teaching. The projects of this period were motivated primarily by the desire to know if I could "solve" the challenge of bringing this or that mechanic or concept into being through programming language, largely without consulting any other sources to see how it was done. The projects are characterized by a lack of concern for code organization, best practices, readability, or maintainability, because my free time was very limited and my chief objective was the satisfaction of knowing I got the idea to work. Most of the time, when I achieved that much of the goal, I was ready to move on to the next logical challenge rather than pore over the details of making a well-rounded product fit for public consumption. 
  
I've taken a little time to restructure some things before posting the code here; nonetheless, much about the program remains primitive or suboptimal as it was originally written. Most notably, I hardcoded my graphics to the size of the screen I worked on at the time, and have had to write some extra code to make them display acceptably on different screen sizes. 

### FILE DESCRIPTIONS
* **sfmlApp:**  Implements `main()`, an SFML window and the abstract app
* **billiards:**  Implements primary graphical elements; physics; game logic
* **objects:**  Small structs representing balls, pockets, etc.
* **jwz:**  C++ utility functions/#defines, shortcuts
* **jwzsfml:**  Like above, but SFML-specific

### BUILDING INSTRUCTIONS
TBA

### TECHNICAL NOTES
I wrote the program on an Intel Mac, where the game is meant to be displayed fullscreen and not in a system window. However, it seems to be confirmed that as of SFML 2.6.1, ARM/Silicon Macs such as the one I am now working on are unable to use fullscreen mode. It sounds like this may be fixed when SFML 2.6.2 is released, but in the meantime, the game will have to run in a system window if being played on an ARM Mac. 
