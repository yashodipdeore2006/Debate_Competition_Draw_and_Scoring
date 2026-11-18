#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <cstring>
#include <sstream>
#include <cstddef>
#include <cstdlib>

using namespace std;

const char DATA_FILE[] = "data/debate_matches.dat";
const int MAX_MATCHES = 100;
const int MIN_SCORE = 0;
const int MAX_SCORE = 100;

// Fixed-size record used only for binary file storage.
// DebateMatch itself contains std::string and must not be written as raw bytes.
struct DebateMatchRecord {
    char matchId[24];
    char roundName[48];
    char teamOne[64];
    char teamTwo[64];
    char debateMotion[200];
    int teamOneScore;
    int teamTwoScore;
};

class DebateMatch {
private:
    string matchId;
    string roundName;
    string teamOne;
    string teamTwo;
    string debateMotion;
    int teamOneScore;
    int teamTwoScore;

    static int totalMatches;
    static int liveObjects;

    static bool copyText(char* destination, size_t capacity,
                         const string& source) {
        if (source.size() >= capacity) {
            return false;
        }

        memcpy(destination, source.c_str(), source.size() + 1);
        return true;
    }

public:
    DebateMatch()
        : matchId(""), roundName(""), teamOne(""), teamTwo(""),
          debateMotion(""), teamOneScore(0), teamTwoScore(0) {
        ++liveObjects;
    }

    DebateMatch(const string& id, const string& round,
                const string& firstTeam, const string& secondTeam,
                const string& motion, int firstScore, int secondScore)
        : matchId(id), roundName(round), teamOne(firstTeam),
          teamTwo(secondTeam), debateMotion(motion),
          teamOneScore(firstScore), teamTwoScore(secondScore) {
        ++totalMatches;
        ++liveObjects;
    }

    DebateMatch(const DebateMatch& other)
        : matchId(other.matchId), roundName(other.roundName),
          teamOne(other.teamOne), teamTwo(other.teamTwo),
          debateMotion(other.debateMotion),
          teamOneScore(other.teamOneScore),
          teamTwoScore(other.teamTwoScore) {
        ++totalMatches;
        ++liveObjects;
    }

    ~DebateMatch() {
        --liveObjects;
    }

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

    inline string getMatchId() const { return matchId; }
    inline string getRoundName() const { return roundName; }
    inline string getTeamOne() const { return teamOne; }
    inline string getTeamTwo() const { return teamTwo; }
    inline string getDebateMotion() const { return debateMotion; }
    inline int getTeamOneScore() const { return teamOneScore; }
    inline int getTeamTwoScore() const { return teamTwoScore; }

    string getWinner() const {
        if (teamOneScore > teamTwoScore) return teamOne;
        if (teamTwoScore > teamOneScore) return teamTwo;
        return "Tie";
    }

    bool toRecord(DebateMatchRecord& record) const {
        memset(&record, 0, sizeof(record));

        if (!copyText(record.matchId, sizeof(record.matchId), matchId) ||
            !copyText(record.roundName, sizeof(record.roundName), roundName) ||
            !copyText(record.teamOne, sizeof(record.teamOne), teamOne) ||
            !copyText(record.teamTwo, sizeof(record.teamTwo), teamTwo) ||
            !copyText(record.debateMotion,
                      sizeof(record.debateMotion), debateMotion)) {
            return false;
        }

        record.teamOneScore = teamOneScore;
        record.teamTwoScore = teamTwoScore;
        return true;
    }

    static DebateMatch fromRecord(const DebateMatchRecord& record) {
        return DebateMatch(record.matchId, record.roundName,
                           record.teamOne, record.teamTwo,
                           record.debateMotion,
                           record.teamOneScore, record.teamTwoScore);
    }

    static int getTotalMatches() { return totalMatches; }
    static int getLiveObjects() { return liveObjects; }

    friend void displayMatch(const DebateMatch& match);
};

int DebateMatch::totalMatches = 0;
int DebateMatch::liveObjects = 0;

// CA1 friend-function requirement: display private class members.
void displayMatch(const DebateMatch& match) {
    cout << "\n----------------------------------------\n"
         << "Match ID : " << match.matchId << '\n'
         << "Round    : " << match.roundName << '\n'
         << "Team 1   : " << match.teamOne << '\n'
         << "Team 2   : " << match.teamTwo << '\n'
         << "Motion   : " << match.debateMotion << '\n'
         << "Score    : " << match.teamOneScore
         << " - " << match.teamTwoScore << '\n'
         << "Result   : " << match.getWinner() << '\n'
         << "----------------------------------------\n";
}

// CA1 array-of-objects requirement. File data is loaded into this array
// when the application starts; the array is also used during this run.
DebateMatch matches[MAX_MATCHES];
int matchCount = 0;

string textInput(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        if (!getline(cin, value)) {
            cout << "\nInput ended. Exiting.\n";
            exit(EXIT_SUCCESS);
        }

        bool nonBlank = false;
        for (size_t i = 0; i < value.size(); ++i) {
            if (!isspace(static_cast<unsigned char>(value[i]))) {
                nonBlank = true;
                break;
            }
        }

        if (nonBlank) return value;
        cout << "Value cannot be blank.\n";
    }
}

int numberInput(const string& prompt, int minimum, int maximum) {
    string line;

    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            cout << "\nInput ended. Exiting.\n";
            exit(EXIT_SUCCESS);
        }

        istringstream input(line);
        long long value;
        char extra;

        if ((input >> value) && !(input >> extra) &&
            value >= minimum && value <= maximum) {
            return static_cast<int>(value);
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

bool ensureDataFile() {
    ifstream existing(DATA_FILE, ios::binary);
    if (existing.good()) return true;

    existing.close();
    ofstream created(DATA_FILE, ios::binary);
    return created.good();
}

bool readRecordAt(fstream& file, streamoff position,
                  DebateMatchRecord& record) {
    file.clear();
    file.seekg(position, ios::beg);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(&record), sizeof(record));
    return file.gcount() == static_cast<streamsize>(sizeof(record));
}

// Loads persistent records into the CA1 fixed array of objects.
// Returns false for an incomplete/corrupted file or capacity overflow.
bool loadMatches() {
    ifstream file(DATA_FILE, ios::binary);
    if (!file) return false;

    matchCount = 0;
    DebateMatchRecord record = {};

    while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (matchCount >= MAX_MATCHES) {
            cout << "The data file contains more than "
                 << MAX_MATCHES << " matches. Increase capacity only "
                 << "with faculty approval.\n";
            return false;
        }

        // Object pointer requirement: point to the next free array object.
        DebateMatch* nextMatch = &matches[matchCount];
        nextMatch->setDetails(record.matchId, record.roundName,
                              record.teamOne, record.teamTwo,
                              record.debateMotion,
                              record.teamOneScore, record.teamTwoScore);
        ++matchCount;
    }

    if (file.gcount() != 0) {
        cout << "Error: incomplete record at the end of the data file.\n";
        return false;
    }

    return true;
}

// File search explicitly uses seekg() and fixed-record byte offsets.
streamoff findMatchPosition(fstream& file, const string& id) {
    file.clear();
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
        if (!readRecordAt(file, position, record)) return -2;
        if (id == record.matchId) return position;
    }

    return -1;
}

bool teamAlreadyUsedInRound(const string& round, const string& team) {
    for (int i = 0; i < matchCount; ++i) {
        const DebateMatch* current = &matches[i]; // object pointer
        if (current->getRoundName() == round &&
            (current->getTeamOne() == team ||
             current->getTeamTwo() == team)) {
            return true;
        }
    }
    return false;
}

void addMatch() {
    cout << "\n========== ADD NEW MATCH ==========\n";

    string id = textInput("Match ID: ");
    string round = textInput("Round name: ");
    string firstTeam = textInput("Team 1: ");
    string secondTeam = textInput("Team 2: ");
    string motion = textInput("Debate motion: ");

    if (!validFieldLengths(id, round, firstTeam, secondTeam, motion)) {
        cout << "One or more fields exceed binary storage capacity.\n";
        return;
    }

    if (firstTeam == secondTeam) {
        cout << "A team cannot debate against itself.\n";
        return;
    }

    if (matchCount >= MAX_MATCHES) {
        cout << "The in-memory match array is full.\n";
        return;
    }

    int firstScore = numberInput("Team 1 score (0-100): ",
                                 MIN_SCORE, MAX_SCORE);
    int secondScore = numberInput("Team 2 score (0-100): ",
                                  MIN_SCORE, MAX_SCORE);

    // Check duplicate ID and file integrity before appending.
    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }

    streamoff existingPosition = findMatchPosition(file, id);
    if (existingPosition == -2) {
        cout << "Data file is incomplete or corrupted. No record added.\n";
        return;
    }
    if (existingPosition >= 0) {
        cout << "Duplicate match ID. Record rejected.\n";
        return;
    }

    if (teamAlreadyUsedInRound(round, firstTeam) ||
        teamAlreadyUsedInRound(round, secondTeam)) {
        cout << "A team is already assigned in this round.\n";
        return;
    }

    // Initialise the next available object using an object pointer.
    DebateMatch* newMatch = &matches[matchCount];
    newMatch->setDetails(id, round, firstTeam, secondTeam,
                         motion, firstScore, secondScore);

    DebateMatchRecord record = {};
    if (!newMatch->toRecord(record)) {
        cout << "Could not convert match to a fixed-size record.\n";
        return;
    }

    file.clear();
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    file.flush();

    if (!file.good()) {
        cout << "File write failed. Match was not added to the array.\n";
        newMatch->setDetails("", "", "", "", "", 0, 0);
        return;
    }

    ++matchCount;
    cout << "Match saved successfully. Result: "
         << newMatch->getWinner() << '\n';
}

void viewAllMatches() {
    if (matchCount == 0) {
        cout << "No saved matches.\n";
        return;
    }

    for (int i = 0; i < matchCount; ++i) {
        const DebateMatch* current = &matches[i];
        displayMatch(*current);
    }

    cout << "Total matches: " << matchCount << '\n';
}

void searchMatch() {
    string id = textInput("Enter match ID to search: ");
    fstream file(DATA_FILE, ios::in | ios::binary);

    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }

    streamoff position = findMatchPosition(file, id);
    if (position == -2) {
        cout << "Data file is incomplete or corrupted.\n";
        return;
    }
    if (position == -1) {
        cout << "Match not found.\n";
        return;
    }

    DebateMatchRecord record = {};
    if (!readRecordAt(file, position, record)) {
        cout << "Could not read the selected record.\n";
        return;
    }

    // Locate the corresponding in-memory object by its unique ID.
    for (int i = 0; i < matchCount; ++i) {
        DebateMatch* current = &matches[i];
        if (current->getMatchId() == id) {
            displayMatch(*current);
            return;
        }
    }

    cout << "Record exists in file but is not loaded in memory.\n";
}

void updateMatchScores() {
    string id = textInput("Enter match ID to update: ");
    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);

    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }

    streamoff position = findMatchPosition(file, id);
    if (position == -2) {
        cout << "Data file is incomplete or corrupted.\n";
        return;
    }
    if (position == -1) {
        cout << "Match not found.\n";
        return;
    }

    DebateMatchRecord record = {};
    if (!readRecordAt(file, position, record)) {
        cout << "Could not read the selected record.\n";
        return;
    }

    cout << "Current score: " << record.teamOne << ' '
         << record.teamOneScore << " - " << record.teamTwoScore
         << ' ' << record.teamTwo << '\n';

    int newFirstScore = numberInput("New Team 1 score (0-100): ",
                                    MIN_SCORE, MAX_SCORE);
    int newSecondScore = numberInput("New Team 2 score (0-100): ",
                                     MIN_SCORE, MAX_SCORE);

    if (numberInput("Confirm update (1 = yes, 0 = no): ", 0, 1) == 0) {
        cout << "Update cancelled.\n";
        return;
    }

    // seekp() positions the output stream at the score fields only.
    streamoff scoreOffset =
        position + static_cast<streamoff>(
            offsetof(DebateMatchRecord, teamOneScore));

    int updatedScores[2] = {newFirstScore, newSecondScore};
    file.clear();
    file.seekp(scoreOffset, ios::beg);

    if (!file) {
        cout << "Could not position file for update.\n";
        return;
    }

    file.write(reinterpret_cast<const char*>(updatedScores),
               sizeof(updatedScores));
    file.flush();

    if (!file.good()) {
        cout << "Score update failed.\n";
        return;
    }

    // Keep the in-memory array synchronised with the persistent file.
    for (int i = 0; i < matchCount; ++i) {
        DebateMatch* current = &matches[i];
        if (current->getMatchId() == id) {
            current->setDetails(record.matchId, record.roundName,
                                record.teamOne, record.teamTwo,
                                record.debateMotion,
                                newFirstScore, newSecondScore);
            break;
        }
    }

    cout << "Scores updated successfully using seekp().\n";
}

void showSummary() {
    int teamOneWins = 0;
    int teamTwoWins = 0;
    int ties = 0;
    long long totalPoints = 0;

    for (int i = 0; i < matchCount; ++i) {
        const DebateMatch* current = &matches[i];

        if (current->getTeamOneScore() > current->getTeamTwoScore()) {
            ++teamOneWins;
        } else if (current->getTeamTwoScore() > current->getTeamOneScore()) {
            ++teamTwoWins;
        } else {
            ++ties;
        }

        totalPoints += current->getTeamOneScore() +
                       current->getTeamTwoScore();
    }

    cout << "\n========== COMPETITION SUMMARY ==========\n"
         << "Total matches       : " << matchCount << '\n'
         << "Team 1-side wins    : " << teamOneWins << '\n'
         << "Team 2-side wins    : " << teamTwoWins << '\n'
         << "Tied matches        : " << ties << '\n'
         << "Total points        : " << totalPoints << '\n'
         << "=========================================\n";
}

void displayMenu() {
    cout << "\n========== DEBATE COMPETITION ==========\n"
         << "1. Add new match\n"
         << "2. View all matches\n"
         << "3. Search match by ID (seekg)\n"
         << "4. Update match scores (seekp)\n"
         << "5. Display competition summary\n"
         << "0. Exit\n"
         << "========================================\n";
}

int main() {
    if (!ensureDataFile()) {
        cerr << "Cannot create/open " << DATA_FILE << ".\n"
             << "Create the data directory in the project folder first.\n";
        return 1;
    }

    if (!loadMatches()) {
        cerr << "Unable to load match records. Check the data file.\n";
        return 1;
    }

    cout << "DEBATE COMPETITION DRAW AND SCORING\n"
         << "Saved matches loaded: " << matchCount << '\n'
         << "Data file: " << DATA_FILE << '\n';

    int choice;
    do {
        displayMenu();
        choice = numberInput("Enter choice (0-5): ", 0, 5);

        switch (choice) {
            case 1: addMatch(); break;
            case 2: viewAllMatches(); break;
            case 3: searchMatch(); break;
            case 4: updateMatchScores(); break;
            case 5: showSummary(); break;
            case 0:
                cout << "Exiting. Records remain saved in the data file.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
