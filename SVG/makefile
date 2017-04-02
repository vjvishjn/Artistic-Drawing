# OBJS specifies which files to compile as part of the project
OBJS = ArtisticLines_SVG.cpp

# CC specifies which compiler we're using
CC = g++


# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ArtisticLines

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) -o $(OBJ_NAME)
	
run : ArtisticLines
		./ArtisticLines
clean:
		rm ArtisticLines
