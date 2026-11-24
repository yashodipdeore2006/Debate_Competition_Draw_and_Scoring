# Debate Competition Draw and Scoring

**Course:** CAP455 — Object-Oriented Programming using C++  
**Student:** Yashodip Prabhakar Deore  
**Reg/Roll Number:** 12620514 / 42  
**Faculty:** Prof. Adarsh Singh  
**Institution:** LPU, School of Computer Application | MCA | 2026–27

A menu-driven C++ console application for recording official and practice debate matches, scores and results. Pairings are entered manually by the organiser; automatic/random draw and bracket generation are not included.

## Implemented assessment scope

### CA1 — Design and Create
- \`DebateMatch\` class with encapsulation, static member, inline getters, friend display function and \`this\` pointer.
- Fixed array of objects, pointers, \`std::string\`, validation, in-memory Create/Read and competition summary.

### CA2 — Persist and Search
- Default, parameterised and copy constructors plus destructor.
- Fixed-size binary records and persistence across restarts.
- Search with \`seekg()\` and score updates with \`seekp()\`.

### CA3 — Inheritance, Overloading and Safe Record Management
- Abstract \`MatchBase\` class with two derived classes: \`DebateMatch\` and \`PracticeMatch\`.
- Virtual functions, virtual destructor, polymorphic base pointers and dynamic \`new\`/\`delete\` for practice-match objects.
- Overloaded \`<<\`, \`>>\`, \`==\`, \`<\` and \`!=\` operators.
- User-defined type conversion between class objects and \`DebateMatchRecord\`.
- Confirmed full-detail update and confirmed delete using a temporary-file rewrite and backup/restore sequence.
- CA2 records are imported once into the tagged CA3 storage file while the original CA2 file is retained.

## Build and run

Run commands from the repository root. The \`data/\` directory must exist.

### Windows PowerShell (MinGW)
\`\`\`powershell
New-Item -ItemType Directory -Force -Path .\data
g++ -std=c++11 -Wall -Wextra -pedantic .\src\main.cpp -o .\debate_tracker.exe
.\debate_tracker.exe
\`\`\`

### Linux/macOS
\`\`\`bash
mkdir -p data
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o debate_tracker
./debate_tracker
\`\`\`

## Runtime data files
- \`data/debate_matches.dat\` — legacy CA2 fixed-record file. If present and the CA3 file does not yet exist, its records are imported as official matches.
- \`data/debate_matches_ca3.dat\` — active tagged CA3 binary file.
- \`data/debate_matches.tmp\` and \`data/debate_matches.bak\` — temporary/rollback files used while updating or deleting records. They are removed after a successful replacement.

The working directory must be the project root because the program uses relative paths. Do not edit runtime binary files manually.

## Repository structure
- \`src/main.cpp\` — complete application source.
- \`data/\` — legacy/runtime data location.
- \`docs/\` — assessment design and progress documentation, diagrams and report references.
- \`tests/\` — test cases and verification record.

## Scope boundaries
Automatic/random draw generation, bracket generation, database connectivity, authentication, simultaneous multi-user editing, CA4 custom exceptions, templates, STL \`vector\`/\`list\` algorithms and advanced backup/export features are not included in this phase.

## Academic integrity
The student must review, understand and be able to explain and modify every submitted line, and disclose AI assistance according to faculty requirements. Manual Windows execution screenshots and any additional faculty-required evidence must be captured by the student.
