Timed Behavior – Task 6

This UPPAAL model simulates the obstacle-handling logic of the line-following robot, including:

Obstacle detection using distance

Color-based routing (red = avoid, blue = push)

Timed transitions using a clock x to simulate delays in real-world actuation (e.g., motor turning, scanning)

⏱️ Timing constraints are modeled using guards (x ≥ N) and invariants (x ≤ N) to reflect Arduino delays.
