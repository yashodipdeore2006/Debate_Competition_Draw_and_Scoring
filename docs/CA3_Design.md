# CA3 Design — Inheritance, Operator Overloading and Safe Record Management

## 1. Purpose

CA3 extends the persisted debate-match application with a small inheritance hierarchy, runtime polymorphism, operator overloading, user-defined type conversion, dynamic memory allocation/deallocation, and confirmed full-detail update/delete operations. Existing CA1 behaviours and CA2 binary persistence/search remain available.

## 2. Class hierarchy

\`MatchBase\` is an abstract base class containing the common match identity, round, team names, motion and scores. It exposes common getters and result calculations and declares a virtual destructor and two virtual functions.

- \`DebateMatch : public MatchBase\` represents an official competition match. The original CA1 fixed array of \`DebateMatch\` objects remains in use for official records.
- \`PracticeMatch : public MatchBase\` represents a practice match. It is allocated dynamically with \`new\` and released with \`delete\` through a \`MatchBase*\` pointer.

Both derived classes override \`getMatchType()\` and \`clone()\`. The polymorphic collection stores \`MatchBase*\` pointers and dispatches the derived behaviour at runtime. The inherited static \`totalMatches\` counter tracks the number of records currently registered in the running process; \`liveObjects\` tracks constructed base subobjects, including the reserved official-object array.

## 3. Operator overloading

| Operator | Behaviour |
|---|---|
| \`<<\` | Displays a match through a \`const MatchBase&\`, including the dynamic type. |
| \`>>\` | Reads match details into either \`DebateMatch\` or \`PracticeMatch\` using line-based input. |
| \`==\` | Compares two matches by their unique match ID. |
| \`<\` | Orders matches by combined score; match ID is the deterministic tie-break. |
| \`!=\` | Logical inverse of equality; used in match comparison. |

The menu's comparison operation exercises identity and ordering operators. The stream operators are used by match creation and display.

## 4. User-defined type conversion

\`DebateMatch\` and \`PracticeMatch\` provide conversion constructors from \`DebateMatchRecord\` through the base-class constructor. \`MatchBase\` provides a conversion operator to \`DebateMatchRecord\`. This supports the class-to-storage and storage-to-class conversions used by persistence.

## 5. Binary storage and CA2 migration

The CA2 file layout is retained in \`LegacyDebateMatchRecord\` solely for migration. The CA3 active file uses a tagged fixed-size \`DebateMatchRecord\` with a \`matchType\` field (\`OFFICIAL\` or \`PRACTICE\`).

- Active file: \`data/debate_matches_ca3.dat\`.
- Legacy file: \`data/debate_matches.dat\`.
- When the CA3 file is absent, the application imports valid legacy CA2 records as \`OFFICIAL\` entries. The original CA2 file is not modified or removed.
- Future application runs use the CA3 file.

The application continues to avoid writing raw \`DebateMatch\`/\`PracticeMatch\` object memory because the classes contain \`std::string\` members. Fixed-width text arrays and primitive integer fields are used for persistence.

## 6. Safe Update and Delete

Updates preserve the unique match ID and match type while allowing the round, team names, motion and scores to change. Input is validated and same-round team conflicts are checked before confirmation. After confirmation, the program writes a replacement file, moves the current file to a backup, and promotes the replacement. If promotion fails, it attempts to restore the backup.

Delete follows the same replacement/backup approach but omits the selected record. Both operations reload the in-memory registry only after successful file replacement. Cancellation leaves the original file unchanged.

## 7. Dynamic memory ownership

The CA1 official-object array remains a fixed array of \`DebateMatch\` objects. \`MatchBase*\` entries refer either to those array objects or to heap-allocated \`PracticeMatch\` objects. A parallel ownership flag identifies dynamically allocated entries. \`clearMatchRegistry()\` deletes only owned pointers; deletion is dispatched safely through the virtual base destructor.

## 8. Requirements mapping

| Requirement | Implementation |
|---|---|
| Base class and two derived classes | \`MatchBase\`, \`DebateMatch\`, \`PracticeMatch\` |
| Virtual function(s) | \`getMatchType()\` and \`clone()\` overrides |
| Virtual destructor | \`virtual ~MatchBase()\` |
| \`<<\` and \`>>\` | Stream display and input overloads |
| \`==\` and \`<\` plus one more | \`==\`, \`<\`, \`!=\` |
| Type conversion | Conversion constructors from record and conversion operator to record |
| \`new\` / \`delete\` | Dynamic \`PracticeMatch\` allocation and virtual deletion |
| Safe Update | Validated, confirmed full-detail rewrite |
| Safe Delete | Confirmed temporary-file rewrite with backup/restore |
| Preserve CA1/CA2 | Fixed official object array, binary persistence and \`seekg()\` record search retained |

## 9. Limitations

Pairings remain manual. The system is single-user and local. The CA3 binary structure is intended for compatible builds and is not a general-purpose portable database. The application retains a maximum of 100 in-memory match pointers per run. CA4 topics such as custom exceptions, templates, STL container algorithms and advanced export/undo are not introduced here.
