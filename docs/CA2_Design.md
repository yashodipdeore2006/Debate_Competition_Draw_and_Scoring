# CA2 Design — Persist and Search

## Objectives
CA2 extends the CA1 Debate Competition Draw and Scoring program with constructors, a destructor, binary fixed-size records, persistence across application restarts, search using `seekg()`, and score updates using `seekp()`.

## Storage model
The application-level `DebateMatch` class continues to use `std::string`. The program does not write raw bytes of `DebateMatch`, because `std::string` owns managed memory and is not a fixed-size binary record. Instead, `DebateMatchRecord` is a separate structure containing fixed-width character arrays and integer scores.

Runtime file: `data/debate_matches.dat`.

| Field | Type |
|---|---|
| matchId | char[24] |
| roundName | char[48] |
| teamOne | char[64] |
| teamTwo | char[64] |
| debateMotion | char[200] |
| teamOneScore | int |
| teamTwoScore | int |

Text exceeding the field capacity is rejected, not silently truncated.

## CA2 OOP additions
- Default constructor.
- Parameterised constructor.
- Copy constructor.
- Destructor.
- Static object counters.

## File operations
- Create appends a complete fixed-size record.
- View reads the saved records.
- Search scans record offsets using `seekg()`.
- Update reads the selected record and overwrites its score fields at the same offset using `seekp()`.
- The binary file remains after the application exits.

## Phase boundary
CA2 updates scores only. Delete, inheritance, operator overloading, dynamic memory, virtual functions, exception handling, templates, STL algorithms and advanced features remain for later CAs.
