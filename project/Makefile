CCC	= g++ -std=c++11 -O3
#-Wall -Wextra -Werror -pedantic -Weffc++
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -g3
NAME = bollspelet

COMPILE_INFO = $(CCC) $(LFLAGS) -o $(NAME)
CC_FILES = main.cc ./Engine/*.cc ./Engine/System/*.cc ./Engine/Graphics/*.cc ./World/*.cc

all:
	$(CCC) $(CC_FILES) -o $(NAME) $(LFLAGS)

clean:
	rm -rf *.o *~ *.gch ./Engine/*.gch ./Engine/System/*.gch ./Engine/Graphics/*.gch ./World/*.gch $(NAME)
