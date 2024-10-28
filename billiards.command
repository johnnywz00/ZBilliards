# navigate to directory containing the source files
clang++ -g -O0 -Wall -std=c++17 -o ZBilliards \
-I. \
# remove this text and the leading hashtag from the 
# next two lines and make sure the paths point to where 
# the SFML binaries are installed
#-rpath /usr/local/lib \
#-L/usr/local/lib \
-lsfml-system.2.6.1 	\
-lsfml-window.2.6.1	\
-lsfml-graphics.2.6.1	\
-lsfml-audio.2.6.1 \
./*.cpp
