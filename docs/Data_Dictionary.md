# Data Dictionary — CA1

| Attribute | Type | Visibility | Meaning | Validation |
|---|---|---|---|---|
| `matchId` | `std::string` | private | Unique match identifier | Non-empty and unique |
| `roundName` | `std::string` | private | Competition round | Non-empty |
| `teamOne` | `std::string` | private | First debating team | Non-empty; cannot equal teamTwo; one match per round |
| `teamTwo` | `std::string` | private | Second debating team | Non-empty; cannot equal teamOne; one match per round |
| `debateMotion` | `std::string` | private | Debate proposition/motion | Non-empty |
| `teamOneScore` | `int` | private | Score awarded to Team 1 | 0–100 |
| `teamTwoScore` | `int` | private | Score awarded to Team 2 | 0–100 |
| `totalMatches` | `int` | private static | Number of records initialised during this run | Updated by `setDetails` |

## Derived value
Winner is the team with the higher score; if scores are equal, the result is `Tie`.

## Unique key
`matchId` is the unique key. Duplicate match IDs are rejected before insertion.

## Scope note
The application records a manually entered pairing and its scores. It does not yet generate random draws, maintain cumulative team standings, or persist records to disk.
