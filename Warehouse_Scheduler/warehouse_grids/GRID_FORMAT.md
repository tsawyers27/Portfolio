# Warehouse Grid Format

All five layouts are 25 columns × 17 rows and share this single-character legend.

| Symbol | Meaning |
| --- | --- |
| `#` | Wall / obstacle (not traversable) |
| `.` | Open, traversable floor |
| `1`, `2`, `3` | Starting locations of robots 1, 2, and 3 |
| `S` | Small package pickup |
| `M` | Medium package pickup |
| `L` | Large package pickup |
| `D` | Delivery zone |

Each warehouse contains **3 robots, 10 packages** (4 small, 3 medium, 3 large), and **2 delivery zones**.

## Parsing conventions

- Map coordinates can use `(x, y) = (column, row)` with `(0, 0)` at the top left.
- Every package symbol represents a separate package; assign IDs while parsing.
- `D` identifies a delivery zone, **not** an automatically assigned destination. Your task data or scheduling setup determines which zone a package uses.
- Robot/package/delivery symbols mark initial locations. Treat their underlying cells as walkable; only `#` blocks travel.
- All marked locations are reachable from each other via four-directional movement.
- These layouts do not dictate task priority, arrival times, degradation rules, or scheduling policy.
