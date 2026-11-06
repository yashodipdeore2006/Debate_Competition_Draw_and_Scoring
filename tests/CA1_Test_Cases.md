# CA1 Test Cases

| Test ID | Scenario | Input / action | Expected result | Actual result |
|---|---|---|---|---|
| TC01 | Create valid match | M001, Round 1, Alpha vs Beta, motion, 65 and 72 | Match stored; Beta is winner | To be recorded after local run |
| TC02 | Duplicate match ID | Create another match with M001 | Duplicate rejected | To be recorded |
| TC03 | Same team on both sides | Alpha vs Alpha | Pairing rejected | To be recorded |
| TC04 | Team already assigned in round | Alpha appears in another Round 1 match | Pairing rejected | To be recorded |
| TC05 | Same team in different round | Alpha appears in Round 2 | Allowed | To be recorded |
| TC06 | Empty required text | Submit blank team or motion | Prompt repeats; no record created | To be recorded |
| TC07 | Negative score | Enter -1 | Validation rejects; asks again | To be recorded |
| TC08 | Score above maximum | Enter 101 | Validation rejects; asks again | To be recorded |
| TC09 | Boundary score | Enter 0 and 100 | Accepted | To be recorded |
| TC10 | Tie | Both teams score 60 | Result is Tie | To be recorded |
| TC11 | Search existing ID | Search M001 | Correct match displayed | To be recorded |
| TC12 | Search missing ID | Search X999 | Not-found message | To be recorded |
| TC13 | View empty list | Select view before creating a match | No-matches message | To be recorded |
| TC14 | Capacity boundary | Attempt match 101 | Storage-full message | To be recorded |

Fill the Actual result column only after executing the test. Attach terminal screenshots as evidence.
