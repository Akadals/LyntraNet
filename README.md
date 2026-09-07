# LyntraNet

> High-performance, cross-platform game server framework for C++.

LyntraNet is an open-source game server framework designed to make
building and operating multiplayer game servers easier.

It provides the networking and server infrastructure required for
multiplayer games while leaving game-specific logic to the developer.

The project is designed with performance, scalability, and developer
experience in mind.

---

## Features

> LyntraNet is currently under active development.

### Core

- C++ based
- CMake based build system
- Modular architecture
- Cross-platform networking
- Windows IOCP
- Linux epoll
- Session management
- Packet processing
- Timer and scheduler
- Thread management
- Memory management
- Logging
- Configuration system
- Plugin / Module architecture

### Server Runtime

LyntraNet is designed around a modular server runtime.

Possible server roles include:

- Gateway Server
- Login Server
- Match Server
- Game Server
- Chat Server
- Custom Server

Server roles are not intended to be tightly coupled to the framework.
Developers can build their own server architecture using LyntraNet's
runtime and core components.

### Networking

Supported networking technologies are planned to include:

- TCP
- UDP
- IOCP
- epoll
- TLS
- Reliable UDP / RUDP

The networking layer is designed to provide a common interface while
allowing platform-specific implementations underneath.

---

## Architecture

LyntraNet is organized into several layers.

```text
                         Client
                            │
                       TCP / UDP / HTTP
                            │
                            ▼
                    ┌───────────────┐
                    │    Gateway    │
                    └───────┬───────┘
                            │
              ┌─────────────┼─────────────┐
              ▼             ▼             ▼
        Login Server   Match Server   Chat Server
                            │
                            ▼
                    ┌───────────────┐
                    │  Game Server  │
                    └───────────────┘
                            │
                            ▼
                    LyntraNet Runtime
                            │
                            ▼
                       Core Layer
