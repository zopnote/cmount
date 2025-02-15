# Cmount

---

## 1. Overview and Motivation

- **Cmount** is an application written in **C**.  
- It natively supports the following languages:  
  - **C** (direct integration)  
  - **Dart** (via extensions)  
  - **Python** (via extensions)  
- Additional languages can be integrated in the future.  
- Core motivations:  
  - **High modularity** and **flexibility** in development.  
  - **Hot Reload** capability during the development phase.  
  - **Unified interface** (Common Protocol) for all extensions.  
  - **AOT compilation** (Ahead Of Time) for optimized release builds.  

---

## 2. Goals

1. **Extensible Development Environment**  
   - Enables adding, updating, and removing **extensions** dynamically.  
   - Extensions can communicate with and depend on each other.  

2. **Multi-Language Interaction**  
   - **C**, **Dart**, and **Python** should support **Hot Reload** during development.  
   - A common **API** and **event system** for communication between languages.  

3. **Common Protocol**  
   - Centralized protocol for **build process and runtime management**.  
   - Standardized interface to facilitate interaction between all extensions (including **modding API**).  

4. **Consistent Build and Release Phases**  
   - **Development Phase**: Focused on **Hot Reload**, JIT, and fast iteration.  
   - **Release Phase**: Focused on **AOT compilation**, performance, and stability.  
   - **Runtime**: Allows installing and managing extensions dynamically.  

5. **Clear Project and Binary Structure**  
   - **Project organization** with defined folder structures.  
   - **Standardized binary output** formats for all supported languages.  

---

## 3. Structure and Components

### 3.1 Cmount Core
- Implemented in **C**.
- Provides fundamental **build logic**, **Hot Reload mechanisms**, and the **Common Protocol**.  
- Allows direct integration of C code (without requiring an extension).

### 3.2 Extensions
- **Dart** and **Python** are integrated as **extensions**.  
- Extensions can interact with and depend on each other (e.g., a Dart extension using a Python extension).  
- Extensions interact with Cmount through a **common interface**:
  - **Events**: Runtime and build-time events.  
  - **API**: Access to the common protocol and other extensions.  

### 3.3 Common Protocol
- Centralized communication and coordination system:  
  - **Build Process** (Hot Reload, JIT, AOT compilation).  
  - **Runtime** (events, extension installation).  
- Defines how extensions register their **capabilities**, **configurations**, and **dependencies**.  
- Enables a consistent **modding API**, allowing applications built with Cmount to support external extensions.

---

## 4. Development Phases

### 4.1 Development Phase
- Focused on **fast iteration** through **Hot Reload** and **JIT**.  
- **C**, **Dart**, and **Python** should work together seamlessly.  
- Event-driven architecture:  
  - **Build-time events** for compilation, testing, and packaging.  
  - **Runtime events** for dynamic updates (Hot Reload).  

### 4.2 Release Phase
- **AOT compilation** for all supported languages (where applicable, e.g., Dart, C).  
- Generation of optimized **binaries** that maintain cross-language compatibility.  
- **Extensions remain installable** even after compilation.  

### 4.3 Runtime
- Executable applications (either **Cmount itself** or software built with Cmount).  
- Supports installing, removing, or updating **extensions** dynamically via **CLI arguments** or **commands**.  
- Uses the **Common Protocol** to expose runtime functions (e.g., Modding API for external extensions).  

---

## 5. Potential Use Cases

1. **Build Toolchains**  
   - Cmount serves as a **development environment** for complex projects using multiple languages.  
   - The **Common Protocol** simplifies scripting, automation, and CI/CD integration.  

2. **Extensible Applications**  
   - Applications that require **dynamic modules** or **plugin systems**.  
   - **Modding support** for games or software with runtime extension capabilities.  

3. **Rapid Prototyping**  
   - Hot Reload for **C, Dart, and Python** speeds up development cycles.  
   - Flexible integration between languages in a shared environment.  

4. **Integration Middleware**  
   - Serves as a **bridge** between different language ecosystems.  
   - Provides a **unified interface** and centralized event system.  

---

## 6. Project Structure and Organization

- **Project Folders**:
  - Source code for **C, Dart, and Python** (organized into separate, clearly named directories).  
  - **Extensions directory**: Stores external modules/plugins managed via the Common Protocol.  
- **Build Artifacts**:
  - Separate **build directories** for development and release builds.  
  - **Configuration files** (e.g., `cmount_config`, `extension_config`) defining dependencies and events.  
- **Executables**:
  - **Cmount binary** (e.g., `cmount` or `cmount.exe`).  
  - **Compiled or interpreted extensions** (language-dependent).  
  - **Final release binaries**, bundling necessary components.  

---

## 7. Common Protocol Details

1. **Registration**  
   - Each extension registers with the Common Protocol and declares its **capabilities** (e.g., event listeners, functions).  
2. **Event System**  
   - **Build-time events**: Compilation, testing, and packaging triggers.  
   - **Runtime events**: Hot Reload, dynamic extension installation, inter-extension interactions.  
3. **API Access**  
   - Common functions for **sending and receiving data**.  
   - Support for **dependencies** and calling functions from other extensions.  
4. **Modding API**  
   - Available for both Cmount itself and applications built with Cmount.  
   - Users can install new extensions via command-line arguments or configuration files.  

---

## 8. Conclusion and Future Outlook

**Cmount** aims to be a **flexible**, **modular**, and **multi-language** platform that supports both **development** and **release** workflows.  
Its **core features** include:

- **Hot Reload** in development for faster iteration.  
- **AOT compilation** in release mode for performance and stability.  
- A **Common Protocol** unifying **C, Dart, Python**, and potential future languages.  
- **Extensions and a Modding API**, allowing applications to remain extensible over time.  

This product definition serves as a **guiding document** for the continued development of Cmount. It establishes the **key components** (Cmount Core, Extensions, Common Protocol, Build Process, Runtime) and their **interactions**, defining the foundation for a **cohesive and extensible software ecosystem**.
