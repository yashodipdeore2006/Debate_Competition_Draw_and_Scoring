# CA3 Test Cases

| ID | Scenario | Expected result |
|---|---|---|
| CA3-01 | Compile using C++11 and warnings | Build succeeds without warnings |
| CA3-02 | Add official debate match | Record persists with \`OFFICIAL\` tag |
| CA3-03 | Add practice debate match | Record persists with \`PRACTICE\` tag and uses dynamic allocation |
| CA3-04 | Restart and view both match types | Correct derived type is reconstructed and displayed |
| CA3-05 | Search official and practice IDs | Correct match is located through \`seekg()\` and displayed |
| CA3-06 | Reuse an existing match ID | Duplicate is rejected |
| CA3-07 | Reuse a team in the same round | Pairing is rejected |
| CA3-08 | Update all editable fields and confirm | Record is replaced and survives restart |
| CA3-09 | Cancel update | Original record remains unchanged |
| CA3-10 | Delete an existing record and confirm | Record is removed and remaining records remain intact |
| CA3-11 | Cancel deletion | Original record remains unchanged |
| CA3-12 | Compare two matches | \`==\`, \`!=\` and \`<\` behave according to documented rules |
| CA3-13 | Add team against itself | Entry rejected |
| CA3-14 | Score outside 0–100 | Invalid score is rejected before file update |
| CA3-15 | Text exceeds fixed field capacity | Entry/update rejected without truncation |
| CA3-16 | Import legacy CA2 data file | Legacy records are copied as official records; source file retained |
| CA3-17 | Incomplete/corrupted record file | Load or rewrite fails safely with an error |
| CA3-18 | Update/delete with file replacement | Original is retained or restored if replacement fails |
