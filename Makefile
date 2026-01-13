# Makefile pour Gtkmm - version avec modules
#
# CC BY-SA Edouard.Thiel@univ-amu.fr - 23/10/2024
#
# Pour tout compiler, tapez : make all
# Pour tout compiler en parallèle, tapez : make -j all
# pour supprimer les .o et l'exécutable : make clean
# Pour tout recompiler : make clean all

SHELL   = /bin/bash
CPP     = g++
RM      = rm -f
CFLAGS  = $$(pkg-config gtkmm-3.0 --cflags) -Wall -O2 --std=c++17  # -g pour gdb
LIBS    = $$(pkg-config gtkmm-3.0 --libs)

# Fichiers à compiler :
CFILES  := $(wildcard *.cpp)
OBJS    := $(CFILES:%.cpp=%.o)

# Nom de l'exécutable à produire :
EXEC1   := domino

# Règle pour fabriquer les .o à partir des .cpp
%.o : %.cpp
	$(CPP) $(CFLAGS) -c $*.cpp

# Déclaration des cibles factices
.PHONY : all clean

# Règle pour tout compiler
all : $(EXEC1)

# Règle de production de l'exécutable
$(EXEC1) : $(OBJS)
	$(CPP) -o $@ $^ $(LIBS)

# Règle de nettoyage - AUTOCLEAN
clean :
	$(RM) *.o *~ $(EXEC1) tmp*.*