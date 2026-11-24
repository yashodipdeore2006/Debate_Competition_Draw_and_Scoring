# CA3 Test Results

## Executed local verification

| Test group | Executed scenario | Result |
|---|---|---|
| Build | \`g++ -std=c++11 -Wall -Wextra -pedantic\` | Passed; no compiler warnings |
| Memory/undefined behaviour | AddressSanitizer + UndefinedBehaviorSanitizer through add/update/delete flow | Passed; no sanitizer diagnostics |
| Official match | Create, restart, view and search official match | Passed |
| Practice match | Create dynamic practice match, restart, view and search | Passed |
| Safe update | Update round/team/motion/scores with confirmation | Passed; updated record reloaded |
| Update cancellation | Confirm update with \`0\` | Passed; original record unchanged |
| Safe delete | Confirm deletion with \`1\` | Passed; selected record removed, remaining records retained |
| Delete cancellation | Confirm deletion with \`0\` | Passed; original record unchanged |
| Validation | Duplicate ID and same-round team reuse | Passed; both rejected |
| Operator overloads | Compare two distinct matches | Passed; comparison used \`==\`, \`!=\` and \`<\` |
| CA2 migration | Import a legacy fixed-size CA2 record | Passed; CA3 tagged record created and legacy file retained |
| Persistent search | Locate records in tagged CA3 file | Passed; seekg-based file scan |

## Manual tests still required

- Run the full application on the student's Windows/MinGW setup.
- Capture genuine screenshots for official/practice insertion, search, update, delete and restart persistence.
- Verify over-capacity field values, extreme score input, empty/corrupt file handling and simulated replacement failure.
- Record the actual computer, compiler version and execution date if requested by the faculty.

Do not mark pending manual tests as passed until the student has run and evidenced them on the target computer. Local scripted verification is not a substitute for faculty demonstration.
