lorenz:	*.cpp *.h
	clang++ -O3 -o lorenz *.cpp -framework OpenGL -framework GLUT

clean:
	rm -rf lorenz
