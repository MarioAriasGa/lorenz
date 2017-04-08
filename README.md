# Animated 3D Simulation of Lorenz Attractor (Mathematical Butterfly Effect / Chaos Theory)

## What the hell is that?

The Lorenz attractor is a set of three differential equations that define a path in tridimentional space:

![](https://wikimedia.org/api/rest_v1/media/math/render/svg/5f993e17e16f1c3ea4ad7031353c61164a226bb8)

Given a 3D point in space (x,y,z) at time t0, you can calculate the next point t1 by using a numerical iterative method (such as Runge-Kutta). If you apply this again and again you can generate a set of points that can be rendered and animated.

So what about it? This path has a "butterfly" shape and exhibits chaotic properties, a small change in the initial point can lead to completely different (and apparently random) paths.

## Compiling

Under OSX, having Xcode installed.

`$ make`
`$ ./lorenz`

## Usage

Space: Play/Pause
ESC: Exit
Mouse: Rotate
Keys 1-5: Several presets.
+/-: Zoom
u: Animation mode (Full graph, start to current, trail)
r/t: Animation speed
y: Restart animation.
o: Line mode (Point/Line/Triangles)
p: Show graph of projection to each axis.

## Screenshots

![](/screenshots/1.png)

