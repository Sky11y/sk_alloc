# introduction

Sky11y's memory allocation library for C. The project will include at least pool allocator and arena allocator.

## Allocators

- Pool allocator use cases:
    - Object pools: Objects have known size, and can be reused
        e.g. game entities, network packets
    - Thread pools: Pre-allocate thread structures to avoid dynamic allocation overhead
- Arena allocator use cases:
    - Tasks that repeat often but are short lived
        - e.g. file parsing, batch requests
    - Temporary objects
        - enemies, particles



