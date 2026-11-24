# Runtime Data Directory

- \`debate_matches.dat\` is the legacy CA2 fixed-size binary file. If the CA3 active file is missing, CA2 records are imported as \`OFFICIAL\` match records.
- \`debate_matches_ca3.dat\` is the active CA3 tagged binary record file, distinguishing \`OFFICIAL\` and \`PRACTICE\` matches.
- \`debate_matches.tmp\` and \`debate_matches.bak\` are temporary files used during safe update/delete replacement. They should not be edited manually.

Run the application from the repository root. Binary data is compiler/layout dependent and should be backed up before manual file operations.
