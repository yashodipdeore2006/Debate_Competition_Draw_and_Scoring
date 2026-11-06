# Debate Competition Draw and Scoring — CA1

**Course:** CAP455 — Object-Oriented Programming using C++  
**Phase:** CA1 — Design & Create  
**Storage:** In-memory fixed-size array; records are lost when the program exits.

## Purpose
A console application for recording debate match pairings (draws), debate motions, team scores, and match results.

## CA1 features
- Create a debate match by entering a unique match ID, round, two teams, motion, and scores.
- Reject duplicate match IDs.
- Reject a team debating itself.
- Prevent a team from being assigned to more than one match in the same round.
- Validate required text and scores (0–100).
- View all matches and search by match ID.
- Display the winner or tie and a basic competition summary.

## CA1 OOP concepts
- `DebateMatch` entity class with private data members.
- Static member `totalMatches` and static getter.
- Inline getter functions.
- Friend function `displayMatch`.
- Array of objects: `DebateMatch matches[MAX_MATCHES]`.
- Pointers to array objects during insertion and search.
- `this` pointer in `setDetails`.
- `std::string` for match, round, team, and motion information.

## Build and run
Requires a standard C++11-or-newer compiler.

```bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o debate_tracker
./debate_tracker
```

On Windows MinGW:
```bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o debate_tracker.exe
debate_tracker.exe
```

## Repository layout
- `docs/CA1_Proposal.docx` — ten-page proposal draft
- `docs/Data_Dictionary.md` — data fields and validation
- `docs/UML_Class_Diagram.md` — class diagram
- `docs/Flowchart.md` — application flow
- `src/main.cpp` — source code
- `tests/CA1_Test_Cases.md` — test plan
- `docs/CA1_Submission_Checklist.md` — submission checklist

## Phase boundary
CA1 implements in-memory Create and Read. File persistence, binary records, update/delete, inheritance, operator overloading, dynamic memory, polymorphism, exception handling, templates, STL containers/algorithms, and advanced features are reserved for later CAs as described in the supplied course guide.

## Important design note
The draw is manually entered as a pairing in CA1; this version does not generate random pairings or calculate a full tournament bracket. Confirm whether your faculty expects automatic/random draw generation before expanding scope.

## Academic integrity
This is a learning draft. Compile, test, personalise, and understand every line before submission. Disclose AI assistance as required by the course guide and obtain faculty approval of the title and scope.
