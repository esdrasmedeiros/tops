# TOPs — Topological Mesh Operators

Original TOPs 1.0 research software for constructing and editing triangulated 2-manifolds, with and without boundary. The source distribution and generated API documentation are preserved from Esdras Medeiros’s academic website.

- [Read the documentation](https://esdrasmedeiros.github.io/tops/)
- [Documentation files](docs/main.html)
- [Research paper](paper/tops_cagd.pdf)
- [Original source archive](docs/tops_1.0.tar.gz)

## Source layout

- `tops/`: half-edge mesh data structures and topological operators.
- `algebra/`: supporting algebra routines.
- `rply/`: PLY input/output library.
- `demo/`: example program and sample mesh.
- `docs/`: original documentation, including examples and API references.

## Original build instructions

From the repository root:

```sh
make install
cd bin
./demo
```

These are historical build instructions from the original distribution; compatibility with current compilers has not been verified. See [README.txt](README.txt) and [compilation notes](docs/code.html).

To browse the documentation locally, run `python3 -m http.server 8000` from the repository root and open `http://localhost:8000/docs/main.html`.

## Publication

Thomas Lewiner, Hélio Lopes, Esdras Medeiros, Geovan Tavares, Luiz Velho. **Topological Mesh Operators.** Computer Aided Geometric Design, 27(1), 1–22, 2010.

## Original notices

The original distribution’s [GPL.txt](GPL.txt), source headers, third-party notices, and [documentation copyright terms](docs/lic.html) are preserved as supplied. No new license has been assigned in this migration.
