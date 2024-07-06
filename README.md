# Tiger Compiler

This project contains the frontend of a Tiger compiler.

Dependencies:
- Nix

To run the project:
```
> nix build .#tc
> nix develop
> ./bootstrap
> ./configure
> make
> ./src/tc <tiger_file>
```
