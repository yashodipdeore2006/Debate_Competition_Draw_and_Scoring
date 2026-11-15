# Debate Competition Draw and Scoring — CA2 Progress Report

**Course:** CAP455 — Object-Oriented Programming using C++  
**Student:** Yashodip Prabhakar Deore  
**Reg/Roll Number:** 12620514 / [42]  
**Faculty:** Prof. Adarsh Singh  
**Institution:** LPU, Computer Application — MCA 2026–27  
**Repository:** https://github.com/yashodipdeore2006/Debate_Competition_Draw_and_Scoring

## Change log

| Version | Phase | Progress |
|---|---|---|
| 1.0 | CA1 | Proposal, in-memory Create/Read, validation, UML and flowchart baseline |
| 2.0 | CA2 | Binary persistence, constructors/destructor, seekg search and seekp score update |

## CA1 baseline
- Create match with ID, round, teams, motion and scores (0–100).
- Reject duplicate IDs, self-pairings and teams already assigned in the same round.
- View all matches, search by ID, determine winner/tie and show a basic summary.
- CA1 used an in-memory array; records were lost on exit.

## CA2 implementation
- Default, parameterised and copy constructors, plus destructor.
- Separate fixed-size binary record structure.
- Persistent file: `data/debate_matches.dat`.
- Search uses `seekg()`.
- Score update uses `seekp()`.
- Records remain available after restarting the application.

## Storage design
`DebateMatch` continues to use `std::string` for application logic. Raw object bytes are not written because `std::string` is a managed object. A separate `DebateMatchRecord` stores fixed-width character arrays and integer scores. Values exceeding field capacity are rejected.

## Diagrams
- [CA2 UML class and storage diagram](UML_CA2.svg)
- [CA2 program flowchart](Flowchart_CA2.svg)

## Testing status
- C++11 warning-enabled compilation: passed in the local build environment.
- Create record and close process: passed.
- Restart and search saved record: passed.
- Update scores using seekp: passed.
- Restart and verify updated scores: passed.
- Manual Windows screenshots and remaining negative/boundary cases: pending student execution.

The scripted checks are preliminary verification, not a substitute for running the application on the student's own computer and capturing evidence.

## Roadmap

| Phase | Scope | Status |
|---|---|---|
| CA1 | Design, Create/Read, validation, UML, flowchart | Baseline |
| CA2 | Constructors/destructor, binary file, seekg, seekp, persistence | Implemented; student screenshots pending |
| CA3 | Inheritance, operators, conversion, new/delete, virtual functions/destructor, safe delete/update | Planned |
| CA4 | Full CRUD, custom exceptions, templates, STL containers/algorithms, advanced feature | Planned |

## Scope note
Pairings remain manually entered; random draw generation is not included. CA2 update is limited to scores and is not full CRUD. Faculty approval of scope and the student's demonstration remain required.

## Academic integrity
The student must understand and be able to explain and modify every submitted line, and disclose AI assistance according to faculty requirements.
