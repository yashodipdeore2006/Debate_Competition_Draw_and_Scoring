# CA2 Test Cases

| ID | Test | Expected result |
|---|---|---|
| CA2-01 | Add valid match | Binary record appended |
| CA2-02 | Restart program | Record remains available |
| CA2-03 | Search existing ID | Found using seekg() |
| CA2-04 | Search missing ID | Not-found message |
| CA2-05 | Duplicate ID | Rejected |
| CA2-06 | Reuse team in same round | Rejected |
| CA2-07 | Update scores and confirm | Existing record overwritten using seekp() |
| CA2-08 | Cancel update | File unchanged |
| CA2-09 | Score outside 0–100 | Rejected |
| CA2-10 | Text exceeds fixed field | Rejected |
| CA2-11 | Truncated trailing record | Integrity warning |
| CA2-12 | C++11 warning-enabled build | Successful |
