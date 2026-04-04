# GeometryLib → CAD Kernel Roadmap

Evolve the current analytic primitives library into a full CAD kernel via 6 sequential phases.
Each phase is independently verifiable and builds directly on the previous one.
The approach mirrors the architecture of production CAD kernels (OpenCASCADE B-Rep model).

Mark items with `[x]` as they are completed.

---

## Phase 0 — Consolidate *(current)*

**Goal:** Close known gaps before adding new code.

### 0.1 — Test coverage gaps

**Point**
- [X] `distance()` and `distance_squared()`
- [X] `operator==` and `operator!=`
- [X] `operator+(Vector)`, `operator-(Vector)`, `operator+=(Vector)`, `operator-=(Vector)`
- [X] `operator-(Point)` → `Vector`

**Vector**
- [X] `operator*=`, `operator/=`
- [X] `operator*` (scalar), `operator/` — `ScalarMultiplicationLeft`, `ScalarMultiplicationRight`, `ScalarDivision`, `ScalarDivisionByZeroThrows`
- [X] `operator==` and `operator!=` — `Equality`, `ApproximateEquality`, `Inequality`
- [X] `unit_x()`, `unit_y()`, `unit_z()` — `UnitVectors`
- [X] `isFinite()` with NaN / infinity components
- [X] `dot()` with a non-zero numeric result
- [X] `angle()` at 0° (parallel) and 180° (anti-parallel)
- [X] Cross product magnitude: `|v1 × v2| = |v1||v2|sin(θ)`
- [X] `operator+=` and `operator-=`

**Line**
- [X] `origin()` accessor value
- [X] `direction()` value correctness (verify direction is NOT normalized on construction)

**Ray**
- [X] `direction()` value correctness for the two-point constructor

**Plane**
- [X] Copy and move construction / assignment
- [X] Oblique (non-axis-aligned) normal
- [X] `contains()` at the tolerance boundary (abs_eps / rel_eps)
- [X] Normal direction convention / winding order of the three-point constructor

### 0.2 — README
- [X] Update project layout: add `ray.hpp` and `plane.hpp` (currently missing)

---

## Phase 1 — Intersection & Spatial Queries

**Goal:** Mathematical query operations that every higher layer depends on.
**Design:** Free functions in `namespace Geometry`. Keeps classes unaware of each other.

### New header: `include/Geometry/Queries/intersect.hpp`
- [x] `intersect(Ray, Plane)`    → `std::optional<Point>`  — ray hits plane at t ≥ 0
- [x] `intersect(Line, Plane)`   → `std::optional<Point>`  — line hits plane
- [x] `intersect(Plane, Plane)`  → `std::optional<Line>`   — two planes intersect in a line
- [x] `intersect(Line, Line)`    → `std::optional<Point>`  — handles skew / parallel / intersecting

### New header: `include/Geometry/Queries/parallel.hpp`
- [x] `is_parallel(Line, Line)`   → `bool` — scale-independent sine test
- [x] `is_parallel(Plane, Plane)` → `bool` — unit-normal sine test

### New header: `include/Geometry/Queries/project.hpp`
- [ ] `project(Point, Line)`     → `Point`   — closest point on line
- [ ] `project(Point, Plane)`    → `Point`   — closest point on plane

### New header: `include/Geometry/Queries/distance.hpp`
- [ ] `distance(Point, Line)`    → `double`  — point-to-line distance
- [ ] `distance(Point, Plane)`   → `double`  — free-function wrapper around `Plane::distance()`
- [ ] `distance(Line, Line)`     → `double`  — skew-line distance

### New source files
- [x] `src/Queries/intersect.cpp`
- [x] `src/Queries/parallel.cpp`
- [ ] `src/Queries/project.cpp`
- [ ] `src/Queries/distance.cpp`

### Tests
- [x] `tests/Queries/test_intersect.cpp` — cover degenerate cases (parallel, skew, coincident) explicitly
- [x] `tests/Queries/test_parallel.cpp`
- [ ] `tests/Queries/test_project.cpp`
- [ ] `tests/Queries/test_distance.cpp`

### CMake
- [x] Add new source files to `cmake/GeometrySources.cmake`
- [x] Add new test files to `cmake/GeometryTestSources.cmake`

### README
- [ ] Update introduction paragraph — mention spatial queries alongside primitives
- [ ] Reconsider the project layout section:
  - The current full file tree will not scale beyond Phase 1 — by Phase 3 it will
    cover dozens of files across Shapes/, Transform/, Curves/, Surfaces/, Topology/, IO/
  - Preferred approach: switch to a **layer-level tree** (one entry per directory,
    with a short description) rather than listing every individual file
  - Example structure to target:
    ```
    include/Geometry/
    ├── Primitives/    # point, vector, line, ray, plane
    ├── Queries/       # intersect, parallel, project, distance
    ├── Shapes/        # segment, triangle, circle, bounding box  (Phase 2)
    └── Transform/     # matrix, transform, coordinate frame      (Phase 3)
    src/               # mirrors include/ layout
    tests/             # mirrors include/ layout
    ```
  - Update `numerical_utils` description to include `k_abs_eps`, `k_rel_eps`, `safe_acos`

---

## Phase 2 — Bounded Shapes (Layer 2 Geometry)

**Goal:** Finite geometric shapes built on top of Layer 1 primitives.

### New class: `Segment`
- [ ] Header `include/Geometry/Shapes/segment.hpp`
- [ ] Source `src/Shapes/segment.cpp`
- [ ] `Segment(Point, Point)` — throws if points are coincident
- [ ] `start()`, `end()` accessors
- [ ] `length()`, `length_squared()`
- [ ] `midpoint()`
- [ ] `direction()` — unit vector from start to end
- [ ] `at(double t)` — parametric point on [0, 1]
- [ ] `contains(Point)` — point lies on segment within tolerance
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Shapes/test_segment.cpp`

### New class: `Triangle`
- [ ] Header `include/Geometry/Shapes/triangle.hpp`
- [ ] Source `src/Shapes/triangle.cpp`
- [ ] `Triangle(Point, Point, Point)` — throws if points are collinear
- [ ] `p0()`, `p1()`, `p2()` accessors
- [ ] `normal()` — unit normal (consistent winding order)
- [ ] `area()`
- [ ] `centroid()`
- [ ] `perimeter()`
- [ ] `contains(Point)` — barycentric test
- [ ] `plane()` — the supporting plane
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Shapes/test_triangle.cpp`

### New class: `Circle`
- [ ] Header `include/Geometry/Shapes/circle.hpp`
- [ ] Source `src/Shapes/circle.cpp`
- [ ] `Circle(Point center, Vector normal, double radius)`
- [ ] `center()`, `normal()`, `radius()` accessors
- [ ] `area()`, `circumference()`
- [ ] `contains(Point)` — point lies within circle (on its plane, within radius)
- [ ] `plane()` — the supporting plane
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Shapes/test_circle.cpp`

### New class: `BoundingBox` (AABB)
- [ ] Header `include/Geometry/Shapes/bounding_box.hpp`
- [ ] Source `src/Shapes/bounding_box.cpp`
- [ ] `BoundingBox(Point min, Point max)`
- [ ] Static factory `BoundingBox::from_points(std::span<Point const>)`
- [ ] `min()`, `max()` accessors
- [ ] `center()`, `diagonal()`, `volume()`, `surface_area()`
- [ ] `contains(Point)`, `contains(BoundingBox)`
- [ ] `intersects(BoundingBox)`
- [ ] `expand(Point)`, `expand(BoundingBox)` — grow to include
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Shapes/test_bounding_box.cpp`

### Extended intersections (add to `Queries/intersect.hpp` / `Queries/intersect.cpp`)
- [ ] `intersect(Ray, Triangle)`     → `std::optional<Point>` — Möller–Trumbore algorithm
- [ ] `intersect(Ray, BoundingBox)`  → `std::optional<Point>` — slab method
- [ ] `intersect(Segment, Plane)`    → `std::optional<Point>`

### CMake
- [ ] Add new source files to `cmake/GeometrySources.cmake`
- [ ] Add new test files to `cmake/GeometryTestSources.cmake`

---

## Phase 3 — Transform & Coordinate Frame

**Goal:** Positional and orientational math required by all topology operations.

### New class: `Matrix3x3`
- [ ] Header `include/Geometry/Transform/matrix3x3.hpp`
- [ ] Source `src/Transform/matrix3x3.cpp`
- [ ] Construction from rows / columns / identity
- [ ] `determinant()`, `inverse()`, `transpose()`
- [ ] `operator*` (matrix × matrix, matrix × vector)
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Transform/test_matrix3x3.cpp`

### New class: `Matrix4x4`
- [ ] Header `include/Geometry/Transform/matrix4x4.hpp`
- [ ] Source `src/Transform/matrix4x4.cpp`
- [ ] Construction from rows / identity
- [ ] `determinant()`, `inverse()`, `transpose()`
- [ ] `operator*` (matrix × matrix)
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Transform/test_matrix4x4.cpp`

### New class: `Transform`
- [ ] Header `include/Geometry/Transform/transform.hpp`
- [ ] Source `src/Transform/transform.cpp`
- [ ] Wraps `Matrix4x4`; correctly handles `Point` (w=1) vs `Vector` (w=0)
- [ ] `apply(Point)`, `apply(Vector)`, `apply(Ray)`, `apply(Plane)`, `apply(Line)`
- [ ] `inverse()`
- [ ] Static factories: `translation(Vector)`, `rotation(Vector axis, double angle)`, `scale(double)`, `scale(Vector)`
- [ ] `compose(Transform)` — multiply transforms
- [ ] `operator==`, `operator!=`
- [ ] Tests `tests/Transform/test_transform.cpp` — round-trip: apply then inverse → identity; verify Point vs Vector transform difference

### New class: `CoordinateFrame`
- [ ] Header `include/Geometry/Transform/coordinate_frame.hpp`
- [ ] Source `src/Transform/coordinate_frame.cpp`
- [ ] `CoordinateFrame(Point origin, Vector x_axis, Vector y_axis, Vector z_axis)`
- [ ] Static factory `CoordinateFrame::world()` — global XYZ frame
- [ ] `origin()`, `x_axis()`, `y_axis()`, `z_axis()` accessors
- [ ] `to_local(Point)`, `to_world(Point)`, `to_local(Vector)`, `to_world(Vector)`
- [ ] `as_transform()` → `Transform`
- [ ] Tests `tests/Transform/test_coordinate_frame.cpp`

### CMake
- [ ] Add new source files to `cmake/GeometrySources.cmake`
- [ ] Add new test files to `cmake/GeometryTestSources.cmake`

---

## Phase 4 — Curves & Surfaces (Geometric Carriers)

**Goal:** Abstract interfaces and concrete implementations that topological entities will reference.
These are purely mathematical — no topology, no boundaries.

### Curves — `include/Geometry/Curves/`

- [ ] `ICurve` (abstract base)
  - [ ] `point_at(double t)` → `Point`
  - [ ] `tangent_at(double t)` → `Vector`
  - [ ] `length()` → `double`
  - [ ] `param_range()` → `[t_min, t_max]`

- [ ] `LineCurve : ICurve` — wraps `Line`, bounded by `[t_min, t_max]`
- [ ] `CircleCurve : ICurve` — center `Point`, normal `Vector`, radius; parameter is angle in [0, 2π]
- [ ] `BSplineCurve : ICurve` — control points, knot vector, degree

### Surfaces — `include/Geometry/Surfaces/`

- [ ] `ISurface` (abstract base)
  - [ ] `point_at(double u, double v)` → `Point`
  - [ ] `normal_at(double u, double v)` → `Vector` (unit)
  - [ ] `param_range()` → `[u_min, u_max, v_min, v_max]`

- [ ] `PlaneSurface : ISurface` — wraps `Plane`, with UV domain
- [ ] `CylindricalSurface : ISurface` — axis `Line`, radius; u = angle, v = height
- [ ] `SphericalSurface : ISurface` — center `Point`, radius; u = longitude, v = latitude
- [ ] `BSplineSurface : ISurface` — control point grid, knot vectors, degrees

### Tests
- [ ] `tests/Curves/test_curves.cpp` — verify `point_at()` against closed-form results
- [ ] `tests/Surfaces/test_surfaces.cpp` — verify `point_at()` and `normal_at()` (unit length, correct direction)

### CMake
- [ ] Add new source files to `cmake/GeometrySources.cmake`
- [ ] Add new test files to `cmake/GeometryTestSources.cmake`

---

## Phase 5 — Topology (B-Rep)

**Goal:** Boundary Representation — connecting geometric carriers with adjacency and connectivity.

### Topological hierarchy (bottom-up)

- [ ] `Vertex` — references a `Point`; accessed via `position()`
- [ ] `Edge` — references `std::shared_ptr<ICurve>` + two `Vertex` bounds + orientation flag
- [ ] `Wire` — ordered, connected closed loop of `Edge`s; validates closure on construction
- [ ] `Face` — references `std::shared_ptr<ISurface>` + one outer `Wire` + zero or more inner `Wire`s (holes) + orientation flag
- [ ] `Shell` — connected set of `Face`s
- [ ] `Solid` — closed `Shell` (watertight volume)

### Builder & validation
- [ ] `BRepBuilder` factory
  - [ ] `make_box(Point min, Point max)` → `Solid`
  - [ ] `make_cylinder(Point base_center, Vector axis, double radius, double height)` → `Solid`
  - [ ] `make_sphere(Point center, double radius)` → `Solid`
  - [ ] `make_face_from_plane(Plane, Wire boundary)` → `Face`
- [ ] `is_valid(Shell)` — basic sanity check
- [ ] `is_closed(Shell)` — every edge is shared by exactly two faces
- [ ] `is_manifold(Shell)` — no non-manifold edges or vertices

### Tests
- [ ] `tests/Topology/test_brep.cpp`
  - [ ] Construct a box; verify 6 faces, 12 edges, 8 vertices
  - [ ] `is_closed()` → true on valid solid
  - [ ] `is_manifold()` → true on valid solid
  - [ ] Traversal: face → edges → vertices round-trip

### CMake
- [ ] Add new source files to `cmake/GeometrySources.cmake`
- [ ] Add new test files to `cmake/GeometryTestSources.cmake`

---

## Phase 6 — I/O & Interoperability

**Goal:** Read and write standard formats; provide a tessellation bridge to renderers.

### STL
- [ ] `io/stl_writer.hpp` / `src/IO/stl_writer.cpp` — ASCII and binary write from a `Triangle` mesh
- [ ] `io/stl_reader.hpp` / `src/IO/stl_reader.cpp` — ASCII and binary read → `std::vector<Triangle>`

### OBJ
- [ ] `io/obj_writer.hpp` / `src/IO/obj_writer.cpp` — write mesh + basic material
- [ ] `io/obj_reader.hpp` / `src/IO/obj_reader.cpp` — read → indexed mesh

### STEP / IGES
- [ ] Evaluate OpenCASCADE wrapper (preferred over from-scratch parser)
- [ ] `io/step_reader.hpp` — thin wrapper around OpenCASCADE `BRep_Builder` → internal `Solid`
- [ ] `io/iges_reader.hpp` — same approach

### Tessellation
- [ ] `tessellate(Face, double chord_tolerance)` → `std::vector<Triangle>`
- [ ] `tessellate(Solid, double chord_tolerance)` → `std::vector<Triangle>`
- [ ] Expose tessellation quality parameter (chord tolerance and angle tolerance)

### Tests
- [ ] `tests/IO/test_stl_io.cpp` — round-trip write then read; compare triangle count and vertex positions within tolerance
- [ ] `tests/IO/test_obj_io.cpp` — round-trip test

### CMake
- [ ] Add new source files to `cmake/GeometrySources.cmake`
- [ ] Add new test files to `cmake/GeometryTestSources.cmake`

---

## Design Decisions

- Free functions for intersections, projections, and distances — avoids cross-class coupling
- Abstract `ICurve` / `ISurface` interfaces — required for polymorphic B-Rep topology
- `std::shared_ptr` for geometry carriers in topological entities — shared ownership (one surface, many faces)
- `GEOM_ASSERT` + `throw` pattern applied consistently in all new code
- STEP / IGES via OpenCASCADE wrapper — not from scratch
- C++20 throughout

## Explicit Scope Exclusions

- No GPU / rendering code — tessellation output feeds an external renderer
- No constraint solver, parametric modelling, or feature tree — pure explicit geometry only
- No implicit surfaces (signed distance fields) — out of scope for a B-Rep kernel
