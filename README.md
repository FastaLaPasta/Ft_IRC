# ft_irc: IRC Server Implementation

ft_irc is a project developed as part of the 42 curriculum. It implements a basic IRC (Internet Relay Chat) server following the official RFC specifications. This project demonstrates network programming in C++ and covers core IRC functionalities such as channel management, user handling, and command processing.

## Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Build & Installation](#build--installation)
- [Usage](#usage)
- [Standards & RFC Compliance](#standards--rfc-compliance)
- [Contributing](#contributing)
- [License](#license)

## Overview

The **ft_irc** project implements a simple IRC server that:
- Manages client connections and user information.
- Processes IRC commands (e.g., joining channels, messaging).
- Supports channel creation and management.
- Follows the IRC RFC guidelines to ensure standard protocol behavior.

This project provided a great opportunity to deepen my understanding of network protocols, socket programming, and the intricacies of real-time communication.


- **Channel.cpp\*** – Manages channel-specific functionalities.
- **clientInfo.cpp** – Handles client-related information.
- **cmd.cpp** – Processes IRC commands.
- **init.cpp** – Contains initialization routines.
- **main.cpp** – Entry point of the server.
- **Makefile** – Automates compilation.
- **mode.cpp** – Manages user/channel modes.
- **Server.\*** – Implements the server logic.
- **User.hpp** – Declares user-related structures and functions.
- **gen.hpp** and **replies.hpp** – Provide auxiliary definitions and standard replies according to the RFC.

## Requirements

- **Compiler:** A C++ compiler supporting C++11 or later.
- **Operating System:** Unix/Linux environment recommended.
- **Other:** Standard development tools (e.g., `make`).

## Build & Installation

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/yourusername/ft_irc.git
    cd ft_irc
    ```

2. **Compile the Project:**
    Simply run:
    ```bash
    make
    ```
    This will compile the source code and generate the executable.

## Usage

After a successful build, run the IRC server:
```bash
./ft_irc [port]


