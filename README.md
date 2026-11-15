# Debate Competition Draw and Scoring

**Course:** CAP455 — Object-Oriented Programming using C++  
**Student:** Yashodip Prabhakar Deore  
**Reg/Roll Number:** 12620514 / [42]  
**Faculty:** Prof. Adarsh Singh  
**Institution:** LPU, Computer Application | MCA | 2026–27

## CA1
Manual debate pairing entry, validation, duplicate ID and same-round team checks, view/search, winner/tie and competition summary. CA1 used an in-memory array.

## CA2 — Persist and Search
- Default, parameterised and copy constructors; destructor.
- Fixed-size binary records in `data/debate_matches.dat`.
- Data survives application restarts.
- Search using `seekg()` and score update using `seekp()`.
- View all saved matches and calculate summary from the binary file.

## Build
```bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o debate_tracker
./debate_tracker
```
On Windows MinGW compile to `debate_tracker.exe` and run `.\debate_tracker.exe`. Run from repository root and ensure the `data/` directory exists.

## CA2 documentation
- `docs/CA2_Design.md`
- `docs/CA2_Progress_Report.md`
- `docs/UML_CA2.svg`
- `docs/Flowchart_CA2.svg`
- `tests/CA2_Test_Cases.md`
- `tests/CA2_Test_Results.md`

CA2 does not implement deletion or later CA3/CA4 topics. Pairings remain manually entered; no random draw is generated.
