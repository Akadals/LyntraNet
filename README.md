# LyntraNet

> **A cross-platform C++ networking framework for high-performance multiplayer games.**

LyntraNet is a performance-oriented, developer-friendly C++ networking framework designed to make multiplayer networking easier to build, optimize, and deploy.

It aims to provide low-level networking infrastructure, reusable framework components, configurable server presets, and high-level APIs for building multiplayer games across different platforms and game engines.

<div align="center">

**C++20 · Cross-Platform · High Performance · Modular · Game Networking**

</div>

---

## Overview

Building multiplayer games requires a large amount of networking infrastructure.

Socket management, asynchronous I/O, connection handling, packet processing, threading, synchronization, server architecture, and eventually higher-level systems such as rooms and matchmaking.

LyntraNet is being designed to provide these components as a single, layered networking framework.

```text
┌─────────────────────────────────────────────┐
│              Game / Application             │
├─────────────────────────────────────────────┤
│               High-Level API                │
├─────────────────────────────────────────────┤
│                   Preset                    │
├─────────────────────────────────────────────┤
│                 Framework                   │
├─────────────────────────────────────────────┤
│                    Core                     │
├─────────────────────────────────────────────┤
│          Platform / OS Networking           │
└─────────────────────────────────────────────┘
```

The goal is to provide:

* High-level APIs for common use cases
* Low-level control for advanced users
* Reusable networking components
* Platform-independent architecture
* Performance-oriented implementations
* Server presets for common architectures

---

## Design Goals

### Performance

Performance is one of the primary design goals of LyntraNet.

The project focuses on:

* Asynchronous I/O
* Zero-copy data paths
* Cache-friendly data structures
* Memory pooling
* Efficient packet processing
* Low synchronization overhead
* Platform-native networking APIs

The initial networking backend targets **Windows IOCP**, with **Linux epoll** planned as the next major platform backend.

---

### Developer Experience

High-performance networking should not require every developer to implement the underlying infrastructure themselves.

LyntraNet aims to provide multiple levels of abstraction.

```text
Simple
  │
  ▼
High-Level API
  │
Preset
  │
Framework
  │
Core
  │
  ▼
Low-Level
```

Developers can use preconfigured systems when they want simplicity, while still being able to access lower-level components when more control is required.

---

### Modularity

LyntraNet is designed as a collection of reusable components rather than a single monolithic networking system.

The major architecture is:

```text
Core
  ↓
Framework
  ↓
Preset
  ↓
High-Level
  ↓
SDK / Deployment / Cloud
```

Each layer has a distinct responsibility and should remain as independent as practical.

---

# Architecture

## Core

Core provides the low-level infrastructure required by the rest of LyntraNet.

```text
Core
├── Platform
├── Network
├── I/O
├── Memory
├── Concurrency
└── Utility
```

Potential responsibilities include:

* Socket abstraction
* TCP / UDP
* TLS / RUDP / QUIC
* IOCP / epoll
* Buffers
* Ring buffers
* Memory pools
* Allocators
* Threading primitives
* Concurrent queues
* Endpoint / address handling
* Error handling

Core intentionally avoids game-specific concepts such as:

* Player
* Room
* Guild
* Matchmaking
* Chat
* MMORPG world systems

Those belong to higher layers.

---

## Framework

Framework provides reusable networking components built on top of Core.

```text
Framework
├── Connection
├── Listener
├── Packet
├── Execution
├── Gameplay Networking
└── Server
```

The Framework layer is intended to provide building blocks such as:

* Connection management
* Session management
* Packet processing
* Serialization
* Event execution
* Worker systems
* Timers
* Rooms
* Replication
* Snapshots
* Interest management
* Server lifecycle

The exact API and module boundaries are still under development.

---

## Presets

Presets are intended to provide preconfigured server architectures built from Framework components.

Possible presets include:

```text
BasicServer
RoomServer
MatchServer
RealtimeServer
GatewayServer
ZoneServer
WorldServer
ServiceServer
```

Presets are not intended to represent individual game genres.

Instead, they represent reusable server roles and architectures.

For example:

```text
FPS
└── MatchServer
    ├── UDP
    ├── Fixed Tick
    ├── Snapshot
    └── Interest Management
```

A larger persistent world could instead be composed from several different server roles:

```text
GatewayServer
      │
      ├── WorldServer
      │
      ├── ZoneServer
      │
      └── ServiceServer
```

The preset system is still being designed and may change substantially during development.

---

# High-Level API

The long-term goal of LyntraNet is to provide high-level APIs for common multiplayer functionality.

Potential areas include:

```text
Client
├── Connection
├── Authentication
├── Room
├── Matchmaking
└── Session

Backend
├── Player Data
├── Friends
├── Guild
├── Chat
├── Ranking
└── Notification
```

These APIs are currently in the **design phase**.

The final interface will be determined after the underlying Core and Framework architecture becomes stable.

---

# Cross-Platform

LyntraNet is designed around a platform abstraction layer so that platform-specific networking implementations can coexist behind a common architecture.

| Platform | Backend |       Status      |
| :------- | :------ | :---------------: |
| Windows  | IOCP    | 🚧 In Development |
| Linux    | epoll   |     📋 Planned    |
| macOS    | TBD     |     📋 Planned    |

The first implementation target is Windows.

Linux support will follow once the core architecture has stabilized.

---

# Protocol Support

| Protocol |       Status      |
| :------- | :---------------: |
| TCP      | 🚧 In Development |
| UDP      |     📋 Planned    |
| TLS      |     📋 Planned    |
| RUDP     |     📋 Planned    |
| QUIC     |     📋 Planned    |

Protocol implementations are intended to remain as independent as practical from higher-level gameplay systems.

---

# Performance

LyntraNet is being developed with performance as a core requirement.

Areas of particular interest include:

* Memory allocation
* Cache locality
* Data movement
* Synchronization
* Thread contention
* Packet processing
* Copy elimination
* Thread ownership
* Object lifetime

The project will include reproducible benchmarks as implementations become stable.

Benchmark results will be published together with their environment and methodology rather than using isolated numbers as marketing claims.

---

# Current Status

LyntraNet is currently in **early development**.

The project is being developed from the lowest-level networking infrastructure upward.

```text
Current
   │
   ▼
Core
   │
   ▼
Framework
   │
   ▼
Presets
   │
   ▼
High-Level API
   │
   ▼
Client SDK / Deployment
   │
   ▼
Cloud Services
```

### Current Focus

* [x] Initial architecture direction
* [x] Core / Framework / Preset layer separation
* [x] High-level API direction
* [x] Server preset architecture
* [ ] TCP networking
* [ ] IOCP implementation
* [ ] Connection / Session framework
* [ ] Packet framework
* [ ] Minimal server
* [ ] Initial server presets
* [ ] Linux epoll backend
* [ ] Client SDK
* [ ] High-level APIs
* [ ] Deployment tools
* [ ] Cloud server management

The roadmap is expected to change as implementation and benchmarking provide new information.

---

# Project Structure

The project is organized around the networking architecture rather than individual game genres.

```text
LyntraNet/
├── include/
│   └── LyntraNet/
├── src/
├── tests/
├── examples/
├── docs/
└── CMakeLists.txt
```

The internal directory structure is still evolving alongside the architecture.

---

# Game Engine Integration

LyntraNet is intended to be usable with multiple game engines and application environments.

Long-term integration targets include:

* Unity
* Unreal Engine
* Godot
* Native C++ applications

The underlying networking library remains independent from any particular game engine.

```text
             LyntraNet
                 │
       ┌─────────┼─────────┐
       │         │         │
     Unity    Unreal     Godot
       │         │         │
       └─────────┼─────────┘
                 │
          Game Application
```

SDK and engine integration are planned for later development.

---

# LyntraNet + Blockia

**Blockia** is intended to serve as a practical testbed for LyntraNet.

```text
          LyntraNet
              │
       Networking Library
              │
              ▼
           Blockia
              │
       Real Integration
              │
              ▼
      Problems / Testing
              │
              ▼
       LyntraNet 개선
```

The purpose is to validate LyntraNet against real gameplay requirements rather than designing the networking framework entirely in isolation.

Blockia is therefore treated primarily as an integration and validation project.

---

# Roadmap

```text
Phase 1
Core Networking
├── Socket
├── TCP
├── IOCP
├── Buffer
├── Memory
└── Concurrency

Phase 2
Framework
├── Connection
├── Session
├── Packet
├── EventLoop
├── Worker
└── Server

Phase 3
Presets
├── BasicServer
├── RoomServer
├── MatchServer
└── RealtimeServer

Phase 4
High-Level API
├── Client
├── Authentication
├── Room
├── Matchmaking
└── Backend Services

Phase 5
Cross-Platform
├── Linux
└── epoll

Phase 6
SDK / Deployment
├── Unity
├── Unreal
├── Godot
├── Docker
└── Server Management

Phase 7
Cloud
├── Server Allocation
├── Region / Zone
├── Instance Lifecycle
├── Monitoring
└── Cloud Management
```

---

# Development Philosophy

### Low-level when necessary

Advanced users should be able to access the underlying networking infrastructure when performance or custom behavior requires it.

### High-level when possible

Common networking tasks should not require every developer to understand the implementation details of asynchronous I/O and connection management.

### Composition over monoliths

Servers should be built from reusable components instead of being locked into one enormous abstraction.

### Platform-aware abstraction

Cross-platform support should abstract common behavior without preventing platform-specific optimizations.

### Server-authoritative networking

Clients are untrusted.

Game-critical state should ultimately be validated and controlled by the server.

---

# Related Projects

| Project       | Role                                 |
| :------------ | :----------------------------------- |
| **LyntraNet** | Networking framework                 |
| **Blockia**   | Game / networking testbed            |
| **HelixRHI**  | Graphics / rendering infrastructure  |
| **AliuxOS**   | Operating system / low-level systems |

These projects explore different layers of game and systems programming, from operating systems and graphics infrastructure to multiplayer networking.

---

# Contributing

LyntraNet is currently under active development and its API may change significantly.

Issues, discussions, benchmarks, experiments, and pull requests are welcome.

For substantial changes, please review the project's architecture and design documentation before contributing.

---

# License

LyntraNet is currently under development.

License information will be finalized before the first stable release.

---

<div align="center">

## LyntraNet

**Build the network. Build the game.**

</div>
