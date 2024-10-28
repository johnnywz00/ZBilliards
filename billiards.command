cd ~/Programming/CPP/Billiards/Billiards;
#
clang++ -g -O0 -Wall -std=c++17 -o Billiards \
\
-I. \
-I/usr/local/include \
-I/Users/johnwz/Programming/CPP/gameheaders \
\
-rpath /usr/local/lib \
-L/usr/local/lib \
-lsfml-system.2.6.1 	\
-lsfml-window.2.6.1	\
-lsfml-graphics.2.6.1	\
-lsfml-audio.2.6.1 \
\
~/Programming/CPP/Billiards/Billiards/*.cpp
