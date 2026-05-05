# Real-Time Implementation of 1-Wire Communication Protocol

⚠️ **Project Status: Work in Progress (Ongoing Development)** ⚠️

## Overview
This repository contains an ongoing project focused on the real-time implementation of the **1-Wire communication protocol**. The goal is to develop a reliable, time-critical software interface capable of handling the strict microsecond timing requirements necessary for communicating with 1-Wire devices (such as temperature sensors, EEPROMs, etc.).

*Please note: This project is currently in the active development phase. The codebase and structure are subject to significant changes as the project approaches its final, polished version.*

## Repository Structure

Currently, the repository is divided into two primary directories to separate the core development from experimental testing:

*   📂 **`Personal_1Wire/` (Main Directory):** 
    This is the core folder of the project. It contains the primary logic, main implementation files, and the most stable working version of the real-time protocol.
*   📂 **`Project_1Wire/` (Testing / Sandbox):** 
    This folder serves as the testing ground. It is used for running test scripts, experimenting with new features, and debugging without affecting the main implementation.

## Future Roadmap
As the project matures, the folder structure and codebase will be thoroughly cleaned up. The final release will feature:
*   A unified, finalized version of the protocol implementation.
*   Comprehensive code documentation and comments.
*   Examples of how to interface the protocol with specific hardware.

---
### 🛠️ Built With
*   **Language:** C
*   **Target Hardware:** STM32f407g-DISC1 with an DS18B20 (temperature sensor)
