# particles
2d gas particle sim

pysics sim based on ideal gas movement
so perfect elastic collision


## issues with the engine
particles can warp throught the edges to the other side but will only ever be computed on one
this may lead to funky things happening, for example a particles warping into a wall, because the wall has no unsticking code bad things might be happening
i should really just make the edges walls too
