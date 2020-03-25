arch/

- This folder contains ONLY architecture DEPENDENT code (lower half drivers,
  other low level peripheral configuration, etc.).
- Here are functions, defines which are dependent by hardware platform.
- Makefile is used in order to bind this arch code with other code.
- No user specific code should be implemented here.
