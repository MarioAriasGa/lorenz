UNAME := $(shell uname)

lorenz:	*.cpp *.h
ifeq ($(UNAME), Darwin)
	clang++ -O3 -o lorenz *.cpp -framework OpenGL -framework GLUT
endif
ifeq ($(UNAME), Linux)
	g++ -O3 -o lorenz *.cpp -lGL -lGLU -lglut
endif

clean:
	rm -rf lorenz
