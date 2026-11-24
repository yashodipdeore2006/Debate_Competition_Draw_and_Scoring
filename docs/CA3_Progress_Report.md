# Debate Competition Draw and Scoring — CA3 Progress Report

**Course:** CAP455 — Object-Oriented Programming using C++  
**Student:** Yashodip Prabhakar Deore  
**Reg/Roll Number:** 12620514 / 42  
**Faculty:** Prof. Adarsh Singh  
**Repository:** https://github.com/yashodipdeore2006/Debate_Competition_Draw_and_Scoring

## Phase progression

| Phase | Scope | Status |
|---|---|---|
| CA1 | In-memory Create/Read, class design, validation, array of objects, pointers, friend/static/inline/this/string concepts | Baseline retained |
| CA2 | Constructors/destructor, binary fixed records, persistence, seekg search and seekp score update | Merged into \`main\` |
| CA3 | Inheritance, operators, type conversion, dynamic allocation, virtual functions/destructor, safe full update/delete | Implemented in this branch; verification recorded below |
| CA4 | Full assessment integration, custom exceptions, templates, STL containers/algorithms and advanced feature | Not started |

## CA3 implementation

- Added abstract \`MatchBase\` with \`DebateMatch\` and \`PracticeMatch\` derived classes.
- Retained a fixed array of official \`DebateMatch\` objects for the CA1 array requirement.
- Added a polymorphic \`MatchBase*\` registry. Practice matches are allocated with \`new\` and released with \`delete\` through the virtual base destructor.
- Added virtual \`getMatchType()\` and \`clone()\` functions, a virtual destructor and inherited static counters.
- Overloaded \`<<\`, \`>>\`, \`==\`, \`<\` and \`!=\`.
- Added conversion constructors from tagged binary records and a conversion operator from match objects to records.
- Kept persistent search on the CA2 \`seekg()\` pattern, adapted to the tagged CA3 record layout.
- Added validated, confirmed updates to match details and scores.
- Added confirmed deletion with temporary-file rewrite and backup/restore handling.
- Preserved the previous CA2 file format as a migration input; imported records become official matches in \`data/debate_matches_ca3.dat\`. The old file is retained.

## Verification record

Local build command:

\`\`\`bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o debate_tracker
\`\`\`

The code compiled successfully without warnings in the local GCC environment. Scripted runs verified:

- Creation and reload of official and practice matches.
- Search of both derived match types after restart using the CA3 file's \`seekg()\` record scan.
- Full-detail update with confirmation and persisted changes.
- Confirmed deletion and removal from the reloaded collection.
- Update cancellation and deletion cancellation without modifying records.
- Duplicate ID rejection and same-round team conflict rejection.
- Comparison operation exercising equality, inequality and ordering operators.
- Migration of a legacy CA2 binary record into the CA3 tagged format while preserving the old file.
- AddressSanitizer and UndefinedBehaviorSanitizer run through add/update/delete without diagnostics.

The complete scenario list and status distinctions are in \`../tests/CA3_Test_Results.md\`. Manual execution on the student's Windows computer and genuine screenshots remain necessary before final submission.

## Run instructions

Run from the repository root. Create \`data/\` if needed, compile with C++11, and execute the generated program. The current CA3 runtime file is \`data/debate_matches_ca3.dat\`.

## Scope note

The project still records pairings manually. Automatic/random draw generation and tournament bracket generation are not implemented. CA4 custom exceptions, templates, STL algorithms and advanced backup/export features are not part of this phase.

## Academic integrity

The student must understand and be prepared to explain and modify the inheritance hierarchy, virtual dispatch, ownership flags, conversion operations, overloaded operators, safe file replacement and tests. Disclose AI assistance according to faculty requirements.
