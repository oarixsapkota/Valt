# Valt

**Valt** is a minimalist, general-purpose programming language heavily inspired by **C** and **Zig**.

### Design Philosophy
Valt aims provides only the most essential language features and leaves everything else — memory management, standard library, runtime services, etc. — to the developer. There is **no Valt-standard library**. On hosted environments, Valt directly uses the platform’s existing **libc**. On bare-metal or freestanding targets (i.e is the primary focus of the language), developers are expected to implement and link whatever facilities they need.

This makes Valt particularly well-suited for:
- Operating system development
- Embedded systems
- Bootloaders
- Low-level libraries

Valt is mos use full in any situation where complete control over the runtime environment is required like OS-devlopment.

### Key Influences
- **C** – proven low-level expressiveness
- **Zig** – familiar syntax, compile-time execution, explicit error handling, no hidden control flow, strong focus on freestanding environments

### Current Status
Valt is in development. The compiler is being written in **c99** and aims to target **LLVM-IR** and uses **LLVM** as it's backend. For now only the lexer ( lexcial analyzer ) is done.

## Example program
### Hello program
Valt has no standard library → we call the kernel directly (example for a hypothetical OS)
```c
func extern syscall_write(int32 file_discriptor, const void* data, uint64 len) ? void;

func main() ? void {
    const char msg[] = "Hello from Valt!\n";
    syscall_write(1, msg, sizeof(msg) - 1);
    while 1; // simple infinite loop
}
```