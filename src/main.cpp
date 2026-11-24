#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <cstring>
#include <sstream>
#include <cstddef>
#include <cstdio>
#include <new>
#include <cstdlib>

using namespace std;

const char DATA_FILE[] = "data/debate_matches_ca3.dat";
const char LEGACY_DATA_FILE[] = "data/debate_matches.dat";
const char TEMP_FILE[] = "data/debate_matches.tmp";
const char BACKUP_FILE[] = "data/debate_matches.bak";
const int MAX_MATCHES = 100;
const int MIN_SCORE = 0;
const int MAX_SCORE = 100;

// Original CA2 record layout. It is retained only to import existing CA2 files.
struct LegacyDebateMatchRecord {
    char matchId[24];
    char roundName[48];
    char teamOne[64];
    char teamTwo[64];
    char debateMotion[200];
    int teamOneScore;
    int teamTwoScore;
};

bool hasNullTerminator(const char* value, size_t capacity);

// CA3 record adds a type tag while retaining fixed-width serialisable fields.
struct DebateMatchRecord {
    char matchType[16];
    char matchId[24];
    char roundName[48];
    char teamOne[64];
    char teamTwo[64];
    char debateMotion[200];
    int teamOneScore;
    int teamTwoScore;
};

class DebateMatch;
void displayMatch(const DebateMatch& match);

class MatchBase {
protected:
    string matchId;
    string roundName;
    string teamOne;
    string teamTwo;
    string debateMotion;
    int teamOneScore;
    int teamTwoScore;
    static int totalMatches;
    static int liveObjects;

    MatchBase()
        : matchId(""), roundName(""), teamOne(""), teamTwo(""),
          debateMotion(""), teamOneScore(0), teamTwoScore(0) {
        ++liveObjects;
    }

    MatchBase(const string& id, const string& round,
              const string& firstTeam, const string& secondTeam,
              const string& motion, int firstScore, int secondScore)
        : matchId(id), roundName(round), teamOne(firstTeam), teamTwo(secondTeam),
          debateMotion(motion), teamOneScore(firstScore), teamTwoScore(secondScore) {
        ++liveObjects;
    }

    explicit MatchBase(const DebateMatchRecord& record)
        : matchId(record.matchId), roundName(record.roundName),
          teamOne(record.teamOne), teamTwo(record.teamTwo),
          debateMotion(record.debateMotion),
          teamOneScore(record.teamOneScore), teamTwoScore(record.teamTwoScore) {
        ++liveObjects;
    }

public:
    virtual ~MatchBase() { --liveObjects; }
    static int getTotalMatches() { return totalMatches; }
    static int getLiveObjects() { return liveObjects; }
    static void resetTotalMatches() { totalMatches = 0; }
    static void registerMatch() { ++totalMatches; }

    virtual string getMatchType() const = 0;
    virtual MatchBase* clone() const = 0;

    void setDetails(const string& id, const string& round,
                    const string& firstTeam, const string& secondTeam,
                    const string& motion, int firstScore, int secondScore) {
        this->matchId = id;
        this->roundName = round;
        this->teamOne = firstTeam;
        this->teamTwo = secondTeam;
        this->debateMotion = motion;
        this->teamOneScore = firstScore;
        this->teamTwoScore = secondScore;
    }

    string getMatchId() const { return matchId; }
    string getRoundName() const { return roundName; }
    string getTeamOne() const { return teamOne; }
    string getTeamTwo() const { return teamTwo; }
    string getDebateMotion() const { return debateMotion; }
    int getTeamOneScore() const { return teamOneScore; }
    int getTeamTwoScore() const { return teamTwoScore; }

    string getWinner() const {
        if (teamOneScore > teamTwoScore) return teamOne;
        if (teamTwoScore > teamOneScore) return teamTwo;
        return "Tie";
    }

    int getCombinedScore() const { return teamOneScore + teamTwoScore; }

    // Conversion operator: a class object can be represented as a storage record.
    operator DebateMatchRecord() const;

    // Retains the original friend-display concept for the official match class.
    friend void displayMatch(const DebateMatch& match);
};

int MatchBase::totalMatches = 0;
int MatchBase::liveObjects = 0;

class DebateMatch : public MatchBase {
public:
    DebateMatch() : MatchBase() {}

    DebateMatch(const string& id, const string& round,
                const string& firstTeam, const string& secondTeam,
                const string& motion, int firstScore, int secondScore)
        : MatchBase(id, round, firstTeam, secondTeam, motion, firstScore, secondScore) {}

    DebateMatch(const DebateMatch& other)
        : MatchBase(other.matchId, other.roundName, other.teamOne, other.teamTwo,
                    other.debateMotion, other.teamOneScore, other.teamTwoScore) {}

    // Conversion constructor from the binary record representation.
    DebateMatch(const DebateMatchRecord& record) : MatchBase(record) {}

    ~DebateMatch() override {}

    string getMatchType() const override { return "OFFICIAL"; }
    MatchBase* clone() const override { return new DebateMatch(*this); }

    friend void displayMatch(const DebateMatch& match);
};

class PracticeMatch : public MatchBase {
public:
    PracticeMatch() : MatchBase() {}

    PracticeMatch(const string& id, const string& round,
                  const string& firstTeam, const string& secondTeam,
                  const string& motion, int firstScore, int secondScore)
        : MatchBase(id, round, firstTeam, secondTeam, motion, firstScore, secondScore) {}

    PracticeMatch(const PracticeMatch& other)
        : MatchBase(other.matchId, other.roundName, other.teamOne, other.teamTwo,
                    other.debateMotion, other.teamOneScore, other.teamTwoScore) {}

    PracticeMatch(const DebateMatchRecord& record) : MatchBase(record) {}

    ~PracticeMatch() override {}

    string getMatchType() const override { return "PRACTICE"; }
    MatchBase* clone() const override { return new PracticeMatch(*this); }
};

bool copyText(char* destination, size_t capacity, const string& source) {
    if (source.size() >= capacity) return false;
    memset(destination, 0, capacity);
    memcpy(destination, source.c_str(), source.size() + 1);
    return true;
}

MatchBase::operator DebateMatchRecord() const {
    DebateMatchRecord record = {};
    copyText(record.matchType, sizeof(record.matchType), getMatchType());
    copyText(record.matchId, sizeof(record.matchId), matchId);
    copyText(record.roundName, sizeof(record.roundName), roundName);
    copyText(record.teamOne, sizeof(record.teamOne), teamOne);
    copyText(record.teamTwo, sizeof(record.teamTwo), teamTwo);
    copyText(record.debateMotion, sizeof(record.debateMotion), debateMotion);
    record.teamOneScore = teamOneScore;
    record.teamTwoScore = teamTwoScore;
    return record;
}

// Stream insertion displays any derived match through a base reference.
ostream& operator<<(ostream& output, const MatchBase& match) {
    output << "\n----------------------------------------\n"
           << "Type     : " << match.getMatchType() << '\n'
           << "Match ID : " << match.getMatchId() << '\n'
           << "Round    : " << match.getRoundName() << '\n'
           << "Team 1   : " << match.getTeamOne() << '\n'
           << "Team 2   : " << match.getTeamTwo() << '\n'
           << "Motion   : " << match.getDebateMotion() << '\n'
           << "Score    : " << match.getTeamOneScore() << " - "
           << match.getTeamTwoScore() << '\n'
           << "Result   : " << match.getWinner() << '\n'
           << "----------------------------------------\n";
    return output;
}

// Equality is based on the unique match ID.
bool operator==(const MatchBase& left, const MatchBase& right) {
    return left.getMatchId() == right.getMatchId();
}

// Additional relational overload: inequality is the inverse of identity equality.
bool operator!=(const MatchBase& left, const MatchBase& right) {
    return !(left == right);
}

// Ordering compares combined score; match ID provides a deterministic tie-break.
bool operator<(const MatchBase& left, const MatchBase& right) {
    if (left.getCombinedScore() != right.getCombinedScore())
        return left.getCombinedScore() < right.getCombinedScore();
    return left.getMatchId() < right.getMatchId();
}

bool isBlank(const string& value) {
    for (size_t i = 0; i < value.size(); ++i) {
        if (!isspace(static_cast<unsigned char>(value[i]))) return false;
    }
    return true;
}

bool readRequiredLine(istream& input, const string& prompt, string& value) {
    while (true) {
        cout << prompt;
        if (!getline(input, value)) return false;
        if (!isBlank(value)) return true;
        cout << "Value cannot be blank. Please try again.\n";
    }
}

bool readIntegerLine(istream& input, const string& prompt,
                     int minimum, int maximum, int& result) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(input, line)) return false;
        istringstream parser(line);
        long long value;
        char extra;
        if ((parser >> value) && !(parser >> extra) &&
            value >= minimum && value <= maximum) {
            result = static_cast<int>(value);
            return true;
        }
        cout << "Enter a whole number from " << minimum
             << " to " << maximum << ".\n";
    }
}

bool validFieldLengths(const string& id, const string& round,
                       const string& firstTeam, const string& secondTeam,
                       const string& motion) {
    return id.size() < sizeof(DebateMatchRecord::matchId) &&
           round.size() < sizeof(DebateMatchRecord::roundName) &&
           firstTeam.size() < sizeof(DebateMatchRecord::teamOne) &&
           secondTeam.size() < sizeof(DebateMatchRecord::teamTwo) &&
           motion.size() < sizeof(DebateMatchRecord::debateMotion);
}

istream& readMatchDetails(istream& input, MatchBase& match) {
    string id, round, firstTeam, secondTeam, motion;
    int firstScore = 0, secondScore = 0;

    if (!readRequiredLine(input, "Match ID: ", id) ||
        !readRequiredLine(input, "Round name: ", round) ||
        !readRequiredLine(input, "Team 1: ", firstTeam) ||
        !readRequiredLine(input, "Team 2: ", secondTeam) ||
        !readRequiredLine(input, "Debate motion: ", motion)) {
        input.setstate(ios::failbit);
        return input;
    }

    if (!validFieldLengths(id, round, firstTeam, secondTeam, motion)) {
        cout << "One or more fields exceed fixed binary storage capacity.\n";
        input.setstate(ios::failbit);
        return input;
    }
    if (firstTeam == secondTeam) {
        cout << "A team cannot debate against itself.\n";
        input.setstate(ios::failbit);
        return input;
    }
    if (!readIntegerLine(input, "Team 1 score (0-100): ", 0, 100, firstScore) ||
        !readIntegerLine(input, "Team 2 score (0-100): ", 0, 100, secondScore)) {
        input.setstate(ios::failbit);
        return input;
    }

    match.setDetails(id, round, firstTeam, secondTeam, motion, firstScore, secondScore);
    return input;
}

// Stream extraction is overloaded for each concrete derived type.
istream& operator>>(istream& input, DebateMatch& match) {
    return readMatchDetails(input, match);
}

istream& operator>>(istream& input, PracticeMatch& match) {
    return readMatchDetails(input, match);
}

// CA1 friend display function is retained for official DebateMatch objects.
void displayMatch(const DebateMatch& match) {
    cout << "\n----------------------------------------\n"
         << "Type     : OFFICIAL\n"
         << "Match ID : " << match.matchId << '\n'
         << "Round    : " << match.roundName << '\n'
         << "Team 1   : " << match.teamOne << '\n'
         << "Team 2   : " << match.teamTwo << '\n'
         << "Motion   : " << match.debateMotion << '\n'
         << "Score    : " << match.teamOneScore << " - "
         << match.teamTwoScore << '\n'
         << "Result   : " << match.getWinner() << '\n'
         << "----------------------------------------\n";
}

// CA1 fixed array-of-objects requirement is retained for official matches.
DebateMatch officialMatches[MAX_MATCHES];
// Polymorphic pointer registry stores both official and dynamically allocated practice matches.
MatchBase* matches[MAX_MATCHES] = {};
bool dynamicallyAllocated[MAX_MATCHES] = {};
int matchCount = 0;
int officialCount = 0;

void clearMatchRegistry() {
    for (int i = 0; i < matchCount; ++i) {
        if (dynamicallyAllocated[i]) {
            delete matches[i]; // virtual destructor dispatches to PracticeMatch.
        }
        matches[i] = NULL;
        dynamicallyAllocated[i] = false;
    }
    matchCount = 0;
    officialCount = 0;
}

bool ensureDataFile() {
    ifstream current(DATA_FILE, ios::binary);
    if (current.good()) return true;
    current.close();

    // Migrate records created by CA2 without modifying the original CA2 file.
    ifstream legacy(LEGACY_DATA_FILE, ios::binary);
    ofstream created(DATA_FILE, ios::binary);
    if (!created) return false;

    if (!legacy.good()) return created.good();

    LegacyDebateMatchRecord oldRecord = {};
    while (legacy.read(reinterpret_cast<char*>(&oldRecord), sizeof(oldRecord))) {
        if (!hasNullTerminator(oldRecord.matchId, sizeof(oldRecord.matchId)) ||
            !hasNullTerminator(oldRecord.roundName, sizeof(oldRecord.roundName)) ||
            !hasNullTerminator(oldRecord.teamOne, sizeof(oldRecord.teamOne)) ||
            !hasNullTerminator(oldRecord.teamTwo, sizeof(oldRecord.teamTwo)) ||
            !hasNullTerminator(oldRecord.debateMotion, sizeof(oldRecord.debateMotion)) ||
            oldRecord.matchId[0] == '\0' || oldRecord.teamOneScore < 0 ||
            oldRecord.teamOneScore > MAX_SCORE || oldRecord.teamTwoScore < 0 ||
            oldRecord.teamTwoScore > MAX_SCORE) {
            cerr << "Invalid legacy CA2 record. Migration stopped.\n";
            created.close();
            std::remove(DATA_FILE);
            return false;
        }
        DebateMatch converted(oldRecord.matchId, oldRecord.roundName,
                              oldRecord.teamOne, oldRecord.teamTwo,
                              oldRecord.debateMotion, oldRecord.teamOneScore,
                              oldRecord.teamTwoScore);
        DebateMatchRecord newRecord = converted;
        created.write(reinterpret_cast<const char*>(&newRecord), sizeof(newRecord));
        if (!created.good()) {
            created.close();
            std::remove(DATA_FILE);
            return false;
        }
    }
    if (!legacy.eof() || legacy.gcount() != 0) {
        cerr << "Legacy CA2 data file contains an incomplete record.\n";
        created.close();
        std::remove(DATA_FILE);
        return false;
    }
    created.flush();
    bool migrationGood = created.good();
    created.close();
    if (!migrationGood) std::remove(DATA_FILE);
    return migrationGood;
}

bool hasNullTerminator(const char* value, size_t capacity) {
    return memchr(value, '\0', capacity) != NULL;
}

bool recordIsValid(const DebateMatchRecord& record) {
    if (!hasNullTerminator(record.matchType, sizeof(record.matchType)) ||
        !hasNullTerminator(record.matchId, sizeof(record.matchId)) ||
        !hasNullTerminator(record.roundName, sizeof(record.roundName)) ||
        !hasNullTerminator(record.teamOne, sizeof(record.teamOne)) ||
        !hasNullTerminator(record.teamTwo, sizeof(record.teamTwo)) ||
        !hasNullTerminator(record.debateMotion, sizeof(record.debateMotion)))
        return false;

    const bool official = strcmp(record.matchType, "OFFICIAL") == 0;
    const bool practice = strcmp(record.matchType, "PRACTICE") == 0;
    return (official || practice) && record.matchId[0] != '\0' &&
           record.roundName[0] != '\0' && record.teamOne[0] != '\0' &&
           record.teamTwo[0] != '\0' && record.debateMotion[0] != '\0' &&
           strcmp(record.teamOne, record.teamTwo) != 0 &&
           record.teamOneScore >= 0 && record.teamOneScore <= MAX_SCORE &&
           record.teamTwoScore >= 0 && record.teamTwoScore <= MAX_SCORE;
}

bool loadMatches() {
    clearMatchRegistry();
    ifstream file(DATA_FILE, ios::binary);
    if (!file) return false;

    MatchBase::resetTotalMatches();
    DebateMatchRecord record = {};
    while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (matchCount >= MAX_MATCHES || !recordIsValid(record)) {
            cerr << "Data file contains too many or invalid match records.\n";
            clearMatchRegistry();
            return false;
        }

        // Conversion constructors rebuild the correct derived object.
        if (strcmp(record.matchType, "OFFICIAL") == 0) {
            if (officialCount >= MAX_MATCHES) {
                clearMatchRegistry();
                return false;
            }
            officialMatches[officialCount].setDetails(
                record.matchId, record.roundName, record.teamOne, record.teamTwo,
                record.debateMotion, record.teamOneScore, record.teamTwoScore);
            matches[matchCount] = &officialMatches[officialCount];
            dynamicallyAllocated[matchCount] = false;
            ++officialCount;
        } else {
            PracticeMatch* practice = new (nothrow) PracticeMatch(record);
            if (practice == NULL) {
                cerr << "Memory allocation failed while loading practice match.\n";
                clearMatchRegistry();
                return false;
            }
            matches[matchCount] = practice;
            dynamicallyAllocated[matchCount] = true;
        }
        ++matchCount;
        MatchBase::registerMatch();
    }

    if (!file.eof() || file.gcount() != 0) {
        cerr << "Incomplete record at the end of the binary file.\n";
        clearMatchRegistry();
        return false;
    }
    return true;
}

int findMatchIndex(const string& id) {
    for (int i = 0; i < matchCount; ++i) {
        if (matches[i]->getMatchId() == id) return i;
    }
    return -1;
}

// Persistent search retains the CA2 seekg() requirement in the tagged CA3 file.
streamoff findMatchFilePosition(const string& id) {
    fstream file(DATA_FILE, ios::in | ios::binary);
    if (!file) return -2;

    file.seekg(0, ios::end);
    streamoff fileSize = file.tellg();
    if (fileSize < 0 ||
        fileSize % static_cast<streamoff>(sizeof(DebateMatchRecord)) != 0) {
        return -2;
    }

    for (streamoff position = 0;
         position < fileSize;
         position += static_cast<streamoff>(sizeof(DebateMatchRecord))) {
        DebateMatchRecord record = {};
        file.clear();
        file.seekg(position, ios::beg);
        if (!file) return -2;
        file.read(reinterpret_cast<char*>(&record), sizeof(record));
        if (file.gcount() != static_cast<streamsize>(sizeof(record))) return -2;
        if (!recordIsValid(record)) return -2;
        if (id == record.matchId) return position;
    }
    return -1;
}

bool teamAlreadyUsedInRound(const string& round, const string& team,
                            const string& exceptId = "") {
    for (int i = 0; i < matchCount; ++i) {
        if (matches[i]->getMatchId() == exceptId) continue;
        if (matches[i]->getRoundName() == round &&
            (matches[i]->getTeamOne() == team || matches[i]->getTeamTwo() == team))
            return true;
    }
    return false;
}

bool appendRecord(const DebateMatchRecord& record) {
    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekg(0, ios::end);
    streamoff size = file.tellg();
    if (size < 0 || size % static_cast<streamoff>(sizeof(DebateMatchRecord)) != 0)
        return false;
    file.clear();
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    file.flush();
    return file.good();
}

bool replaceFileSafely(const string& targetId,
                       const DebateMatchRecord* replacement,
                       bool deleteTarget) {
    ifstream input(DATA_FILE, ios::binary);
    if (!input) return false;
    ofstream output(TEMP_FILE, ios::binary | ios::trunc);
    if (!output) return false;

    bool found = false;
    DebateMatchRecord record = {};
    while (input.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (!recordIsValid(record)) {
            input.close(); output.close(); std::remove(TEMP_FILE);
            return false;
        }
        if (record.matchId == string(targetId.c_str())) {
            if (found) {
                input.close(); output.close(); std::remove(TEMP_FILE);
                return false;
            }
            found = true;
            if (!deleteTarget && replacement != NULL)
                output.write(reinterpret_cast<const char*>(replacement), sizeof(*replacement));
        } else {
            output.write(reinterpret_cast<const char*>(&record), sizeof(record));
        }
        if (!output.good()) break;
    }
    bool validEnd = input.eof() && input.gcount() == 0;
    input.close(); output.flush();
    bool outputGood = output.good();
    output.close();

    if (!found || !validEnd || !outputGood) {
        std::remove(TEMP_FILE);
        return false;
    }

    std::remove(BACKUP_FILE);
    if (std::rename(DATA_FILE, BACKUP_FILE) != 0) {
        std::remove(TEMP_FILE);
        return false;
    }
    if (std::rename(TEMP_FILE, DATA_FILE) != 0) {
        std::rename(BACKUP_FILE, DATA_FILE);
        std::remove(TEMP_FILE);
        return false;
    }
    std::remove(BACKUP_FILE);
    return true;
}

void addMatch() {
    if (matchCount >= MAX_MATCHES) {
        cout << "The match capacity of " << MAX_MATCHES << " has been reached.\n";
        return;
    }

    cout << "\n1. Official debate match\n2. Practice debate match\n";
    int type = 0;
    if (!readIntegerLine(cin, "Select match type: ", 1, 2, type)) return;

    DebateMatch officialInput;
    PracticeMatch practiceInput;
    MatchBase* candidate = NULL;
    bool ownsCandidate = false;

    if (type == 1) {
        if (!(cin >> officialInput)) {
            cin.clear();
            cout << "Match entry cancelled because a field was invalid.\n";
            return;
        }
        candidate = &officialInput;
    } else {
        if (!(cin >> practiceInput)) {
            cin.clear();
            cout << "Match entry cancelled because a field was invalid.\n";
            return;
        }
        candidate = &practiceInput;
    }

    if (findMatchIndex(candidate->getMatchId()) >= 0) {
        cout << "Duplicate match ID. Record rejected.\n";
        return;
    }
    if (teamAlreadyUsedInRound(candidate->getRoundName(), candidate->getTeamOne()) ||
        teamAlreadyUsedInRound(candidate->getRoundName(), candidate->getTeamTwo())) {
        cout << "A team is already assigned to another match in this round.\n";
        return;
    }

    DebateMatchRecord record = *candidate; // user-defined conversion to storage record.
    if (type == 2) {
        candidate = new (nothrow) PracticeMatch(practiceInput);
        ownsCandidate = true;
        if (candidate == NULL) {
            cout << "Could not allocate memory for the practice match.\n";
            return;
        }
    }

    if (!appendRecord(record)) {
        if (ownsCandidate) delete candidate;
        cout << "Could not append the match to the binary file.\n";
        return;
    }

    if (type == 1) {
        officialMatches[officialCount].setDetails(
            candidate->getMatchId(), candidate->getRoundName(),
            candidate->getTeamOne(), candidate->getTeamTwo(),
            candidate->getDebateMotion(), candidate->getTeamOneScore(),
            candidate->getTeamTwoScore());
        matches[matchCount] = &officialMatches[officialCount];
        dynamicallyAllocated[matchCount] = false;
        ++officialCount;
    } else {
        matches[matchCount] = candidate;
        dynamicallyAllocated[matchCount] = true;
    }
    ++matchCount;
    MatchBase::registerMatch();
    cout << "Match saved successfully.\n" << *matches[matchCount - 1];
}

void viewAllMatches() {
    if (matchCount == 0) {
        cout << "No saved matches.\n";
        return;
    }
    for (int i = 0; i < matchCount; ++i) {
        if (matches[i]->getMatchType() == "OFFICIAL") {
            const DebateMatch* official = dynamic_cast<const DebateMatch*>(matches[i]);
            if (official != NULL) displayMatch(*official);
        } else {
            cout << *matches[i];
        }
    }
    cout << "Total matches: " << matchCount << '\n';
}

void searchMatch() {
    string id;
    if (!readRequiredLine(cin, "Enter match ID to search: ", id)) return;
    streamoff position = findMatchFilePosition(id);
    if (position == -2) {
        cout << "Could not search the binary data file; check its integrity.\n";
        return;
    }
    if (position == -1) {
        cout << "Match not found.\n";
        return;
    }

    int index = findMatchIndex(id);
    if (index >= 0) cout << *matches[index];
    else cout << "Match exists in the file but is not loaded in memory.\n";
}

void updateMatch() {
    string id;
    if (!readRequiredLine(cin, "Enter match ID to update: ", id)) return;
    int index = findMatchIndex(id);
    if (index < 0) {
        cout << "Match not found.\n";
        return;
    }

    MatchBase* current = matches[index];
    cout << "Current record:\n" << *current;
    string round, firstTeam, secondTeam, motion;
    if (!readRequiredLine(cin, "New round name: ", round) ||
        !readRequiredLine(cin, "New Team 1 name: ", firstTeam) ||
        !readRequiredLine(cin, "New Team 2 name: ", secondTeam) ||
        !readRequiredLine(cin, "New debate motion: ", motion)) return;
    if (firstTeam == secondTeam) {
        cout << "A team cannot debate against itself. Update rejected.\n";
        return;
    }
    if (!validFieldLengths(id, round, firstTeam, secondTeam, motion)) {
        cout << "One or more fields exceed fixed storage capacity. Update rejected.\n";
        return;
    }
    if (teamAlreadyUsedInRound(round, firstTeam, id) ||
        teamAlreadyUsedInRound(round, secondTeam, id)) {
        cout << "A team is already assigned in another match in this round. Update rejected.\n";
        return;
    }
    int firstScore = 0, secondScore = 0;
    if (!readIntegerLine(cin, "New Team 1 score (0-100): ", 0, 100, firstScore) ||
        !readIntegerLine(cin, "New Team 2 score (0-100): ", 0, 100, secondScore)) return;
    int confirmation = 0;
    if (!readIntegerLine(cin, "Confirm update (1 = yes, 0 = no): ", 0, 1, confirmation)) return;
    if (confirmation == 0) {
        cout << "Update cancelled. No data was changed.\n";
        return;
    }

    DebateMatchRecord updated = *current;
    copyText(updated.roundName, sizeof(updated.roundName), round);
    copyText(updated.teamOne, sizeof(updated.teamOne), firstTeam);
    copyText(updated.teamTwo, sizeof(updated.teamTwo), secondTeam);
    copyText(updated.debateMotion, sizeof(updated.debateMotion), motion);
    updated.teamOneScore = firstScore;
    updated.teamTwoScore = secondScore;

    if (!replaceFileSafely(id, &updated, false)) {
        cout << "Update failed. Original file was retained.\n";
        return;
    }
    if (!loadMatches()) {
        cout << "Update was written, but the in-memory reload failed. Restart and inspect the data file.\n";
        return;
    }
    cout << "Match updated successfully.\n";
}

void deleteMatch() {
    string id;
    if (!readRequiredLine(cin, "Enter match ID to delete: ", id)) return;
    int index = findMatchIndex(id);
    if (index < 0) {
        cout << "Match not found.\n";
        return;
    }
    cout << "Selected record:\n" << *matches[index];
    int confirmation = 0;
    if (!readIntegerLine(cin, "Confirm deletion (1 = yes, 0 = no): ", 0, 1, confirmation)) return;
    if (confirmation == 0) {
        cout << "Deletion cancelled. No data was changed.\n";
        return;
    }
    if (!replaceFileSafely(id, NULL, true)) {
        cout << "Deletion failed. Original file was retained.\n";
        return;
    }
    if (!loadMatches()) {
        cout << "Deletion was written, but the in-memory reload failed. Restart and inspect the data file.\n";
        return;
    }
    cout << "Match deleted successfully.\n";
}

void showSummary() {
    int official = 0, practice = 0, firstWins = 0, secondWins = 0, ties = 0;
    long long totalPoints = 0;
    for (int i = 0; i < matchCount; ++i) {
        if (matches[i]->getMatchType() == "OFFICIAL") ++official;
        else ++practice;
        if (matches[i]->getTeamOneScore() > matches[i]->getTeamTwoScore()) ++firstWins;
        else if (matches[i]->getTeamTwoScore() > matches[i]->getTeamOneScore()) ++secondWins;
        else ++ties;
        totalPoints += matches[i]->getCombinedScore();
    }
    cout << "\n========== COMPETITION SUMMARY ==========\n"
         << "Total matches       : " << matchCount << '\n'
         << "Official matches    : " << official << '\n'
         << "Practice matches    : " << practice << '\n'
         << "Team 1-side wins    : " << firstWins << '\n'
         << "Team 2-side wins    : " << secondWins << '\n'
         << "Tied matches        : " << ties << '\n'
         << "Total points        : " << totalPoints << '\n'
         << "Current-process registered matches: " << MatchBase::getTotalMatches() << '\n'
         << "Live MatchBase objects (includes reserved official array slots): "
         << MatchBase::getLiveObjects() << '\n'
         << "=========================================\n";
}

void compareMatches() {
    string firstId, secondId;
    if (!readRequiredLine(cin, "First match ID: ", firstId) ||
        !readRequiredLine(cin, "Second match ID: ", secondId)) return;
    int first = findMatchIndex(firstId), second = findMatchIndex(secondId);
    if (first < 0 || second < 0) {
        cout << "Both match IDs must exist.\n";
        return;
    }
    if (*matches[first] == *matches[second]) {
        cout << "Both identifiers refer to the same match.\n";
    } else if (*matches[first] < *matches[second]) {
        cout << firstId << " has a lower combined score than " << secondId << ".\n";
    } else if (*matches[first] != *matches[second]) {
        cout << firstId << " has a higher combined score than " << secondId << ".\n";
    }
}

void displayMenu() {
    cout << "\n========== DEBATE COMPETITION ==========\n"
         << "1. Add match (official or practice)\n"
         << "2. View all matches\n"
         << "3. Search match by ID\n"
         << "4. Update match details and scores\n"
         << "5. Delete match\n"
         << "6. Display competition summary\n"
         << "7. Compare two matches (operator overloads)\n"
         << "0. Exit\n"
         << "========================================\n";
}

int main() {
    if (!ensureDataFile()) {
        cerr << "Cannot create/open " << DATA_FILE
             << ". Run the program from the project root and ensure data/ exists.\n";
        return 1;
    }
    if (!loadMatches()) {
        cerr << "Unable to load match records. Check the binary data file.\n";
        return 1;
    }

    cout << "DEBATE COMPETITION DRAW AND SCORING — CA3\n"
         << "Saved matches loaded: " << matchCount << '\n'
         << "Data file: " << DATA_FILE << '\n';

    int choice = 0;
    do {
        displayMenu();
        if (!readIntegerLine(cin, "Enter choice (0-7): ", 0, 7, choice)) break;
        switch (choice) {
            case 1: addMatch(); break;
            case 2: viewAllMatches(); break;
            case 3: searchMatch(); break;
            case 4: updateMatch(); break;
            case 5: deleteMatch(); break;
            case 6: showSummary(); break;
            case 7: compareMatches(); break;
            case 0: cout << "Exiting. Records remain saved.\n"; break;
        }
    } while (choice != 0);

    clearMatchRegistry();
    return 0;
}
