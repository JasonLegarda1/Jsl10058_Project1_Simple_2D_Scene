# Jason Legarda Project 1 — Simple 2D Scene

A soccer-themed raylib animation: Messi dribbles a ball across the field, kicks it into the goalnet, and a Victory Royale banner pops up for the celebration. The sky breathes blue during play and flashes gold on the goal.

## Build and run

```sh
make
make run
```

Requires raylib installed via Homebrew (`brew install raylib`). macOS Apple Silicon.

## Scene

An 11-second cycle in three phases:

1. **Run (0 – 5.25s)** — Messi runs rightward along the grass line with a running bounce, the ball dribbles alongside him with its own wobble and bounce, spinning and pulsing in scale.
2. **Kick (5.25 – 7s)** — Messi plants and swings, the ball flies in a straight angled line into the top of the goalnet.
3. **Celebration (7 – 11s)** — The Victory Royale banner scales up with a curve and wobbles in a circle, the sky flashes gold.
