# Spacegrid

An experimental game engine with extendable, easy-to-understand data structure and lua scripting.

## Instructions

### Build & Run

```bash
  # Python env setup
  $ virtualenv .venv 
  $ source .venv/bin/activate

  # Install python dependencies
  $ pip install requirements.txt

  # Build
  $ ./build_init.sh
  $ ./build.sh

  # Run tests
  $ ./test.sh    
   
  # Run game
  $ ./run.sh
```

## Dependencies

### Build

- Python virtualenv (optional)
- Meson
- Python 3 (for asset building)
  - watchdog
- Git LFS (used for binary assets)
- Blender (for assets, must be in $PATH)
- Doxygen

### Runtime

- SDL2
- Lua
- SDL2_mixer
- GLEW
- glm
