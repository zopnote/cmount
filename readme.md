# ![cmount_logo](.github/images/cmount.png)
Unifying application for efficient and C-dependent 
program development.

Projects that built with Cmount profits of a modern and productive
development cycle, easy extensibility, performance as well as cross-platform targets. 
Users of software built with Cmount also profits by easy modding capabilities 
(as long the developer want) made possible by Cmount's unification protocol.

And no worries, Cmount is well documented.

---
# Goals

### 1. Extensible Development Environment
- Enables adding, updating, and removing **extensions** dynamically.
- Extensions can communicate with and depend on each other.

### 2. Multi-Language Interaction
- **C**, **Dart**, and **Python** should support **Hot Reload** during development.
- A common **API** and **event system** for communication between languages.

### 3. Common Protocol
- Centralized protocol for **build process and runtime management**.
- Standardized interface to facilitate interaction between all extensions (including **modding API**).

### 4. Consistent Build and Release Phases
- **Development Phase**: Focused on **Hot Reload**, JIT, and fast iteration.
- **Release Phase**: Focused on **AOT compilation**, performance, and stability.
- **Runtime**: Allows installing and managing extensions dynamically.

### 5. Clear Project and Binary Structure
- **Project organization** with defined folder structures.
- **Standardized binary output** formats for all supported languages.
---
# Structure and Components
### 1. Core
- Implemented in **C**.
- Provides fundamental **build logic**, **Hot Reload mechanisms**, and the **Common Protocol**.
- Allows direct integration of C code (without requiring an extension).

### 2. Extensions
- **Dart** and **Python** are integrated as **extensions**.
- Extensions can interact with and depend on each other (e.g., a Dart extension using a Python extension).
- Extensions interact with Cmount through a **common interface**:
    - **Events**: Runtime and build-time events.
    - **API**: Access to the common protocol and other extensions.

### 3. Common Protocol
- Centralized communication and coordination system:
    - **Build Process** (Hot Reload, JIT, AOT compilation).
    - **Runtime** (events, extension installation).
- Defines how extensions register their **capabilities**, **configurations**, and **dependencies**.
- Enables a consistent **modding API**, allowing applications built with Cmount to support external extensions.

---

## Participated components
Cmount is created for **Fireworks**, a graphic engine framework.
As result, Cmount is actually intended to be extended and is participated
in its components as shared libraries.
