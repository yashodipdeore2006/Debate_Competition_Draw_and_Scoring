#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

const int MAX_MATCHES = 100;
const int MAX_SCORE = 100;

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

public:
    DebateMatch()
        : matchId(""), roundName(""), teamOne(""), teamTwo(""),
          debateMotion(""), teamOneScore(0), teamTwoScore(0) {}

    // Initialise the current object using the this pointer.
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
        ++totalMatches;
    }

    // Inline getters.
    inline string getMatchId() const { return matchId; }
    inline string getRoundName() const { return roundName; }
    inline string getTeamOne() const { return teamOne; }
    inline string getTeamTwo() const { return teamTwo; }
    inline int getTeamOneScore() const { return teamOneScore; }
    inline int getTeamTwoScore() const { return teamTwoScore; }

    inline string getWinner() const {
        if (teamOneScore > teamTwoScore) return teamOne;
        if (teamTwoScore > teamOneScore) return teamTwo;
        return "Tie";
    }

    static int getTotalMatches() { return totalMatches; }

    // Friend function can access private data.
    friend void displayMatch(const DebateMatch& match);
};

int DebateMatch::totalMatches = 0;

void displayMatch(const DebateMatch& match) {
    cout << "\n============================================\n";
    cout << "Match ID       : " << match.matchId << '\n';
    cout << "Round          : " << match.roundName << '\n';
    cout << "Team 1         : " << match.teamOne << '\n';
    cout << "Team 2         : " << match.teamTwo << '\n';
    cout << "Debate Motion  : " << match.debateMotion << '\n';
    cout << "Score          : " << match.teamOne << " " << match.teamOneScore
         << " - " << match.teamTwoScore << " " << match.teamTwo << '\n';
    cout << "Result         : " << match.getWinner() << '\n';
    cout << "============================================\n";
}

string readNonEmptyLine(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        bool nonSpace = false;
        for (char ch : value) {
            if (!isspace(static_cast<unsigned char>(ch))) {
                nonSpace = true;
                break;
            }
        }
        if (nonSpace) return value;
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int readInteger(const string& prompt, int minimum, int maximum) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Enter a whole number from "
             << minimum << " to " << maximum << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int findMatchIndex(const DebateMatch matches[], int count, const string& id) {
    for (int i = 0; i < count; ++i) {
        if (matches[i].getMatchId() == id) return i;
    }
    return -1;
}

bool teamAlreadyDrawnInRound(const DebateMatch matches[], int count,
                             const string& round, const string& team) {
    for (int i = 0; i < count; ++i) {
        if (matches[i].getRoundName() == round &&
            (matches[i].getTeamOne() == team || matches[i].getTeamTwo() == team)) {
            return true;
        }
    }
    return false;
}

void createDrawAndScore(DebateMatch matches[], int& count) {
    if (count >= MAX_MATCHES) {
        cout << "\nMatch storage is full. Cannot create another match.\n";
        return;
    }

    cout << "\n========== CREATE DEBATE DRAW & ENTER SCORE ==========\n";
    string id = readNonEmptyLine("Enter unique match ID: ");

    if (findMatchIndex(matches, count, id) != -1) {
        cout << "Duplicate match ID. Match was not created.\n";
        return;
    }

    string round = readNonEmptyLine("Enter round name (e.g., Preliminary 1): ");
    string firstTeam = readNonEmptyLine("Enter Team 1 name: ");
    string secondTeam = readNonEmptyLine("Enter Team 2 name: ");

    if (firstTeam == secondTeam) {
        cout << "A team cannot debate against itself. Match was not created.\n";
        return;
    }

    if (teamAlreadyDrawnInRound(matches, count, round, firstTeam) ||
        teamAlreadyDrawnInRound(matches, count, round, secondTeam)) {
        cout << "One of these teams is already assigned to a match in this round.\n";
        cout << "Match was not created.\n";
        return;
    }

    string motion = readNonEmptyLine("Enter debate motion/topic: ");
    int firstScore = readInteger("Enter Team 1 score (0-100): ", 0, MAX_SCORE);
    int secondScore = readInteger("Enter Team 2 score (0-100): ", 0, MAX_SCORE);

    // Pointer to the next free object in the array.
    DebateMatch* matchPointer = &matches[count];
    matchPointer->setDetails(id, round, firstTeam, secondTeam,
                             motion, firstScore, secondScore);
    ++count;

    cout << "\nDebate draw and score saved successfully in memory.\n";
    cout << "Winner / result: " << matchPointer->getWinner() << '\n';
}

void viewAllMatches(const DebateMatch matches[], int count) {
    cout << "\n========== ALL DEBATE MATCHES ==========\n";
    if (count == 0) {
        cout << "No debate matches have been recorded yet.\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        displayMatch(matches[i]);
    }
    cout << "Total matches: " << count << '\n';
}

void searchMatch(const DebateMatch matches[], int count) {
    cout << "\n========== SEARCH DEBATE MATCH ==========\n";
    if (count == 0) {
        cout << "No matches available to search.\n";
        return;
    }

    string id = readNonEmptyLine("Enter match ID: ");
    int index = findMatchIndex(matches, count, id);

    if (index == -1) {
        cout << "No match found with ID: " << id << '\n';
    } else {
        const DebateMatch* matchPointer = &matches[index];
        displayMatch(*matchPointer);
    }
}

void showCompetitionSummary(const DebateMatch matches[], int count) {
    if (count == 0) {
        cout << "\nNo matches available for summary.\n";
        return;
    }

    int teamOneWins = 0;
    int teamTwoWins = 0;
    int ties = 0;
    int totalPoints = 0;

    for (int i = 0; i < count; ++i) {
        if (matches[i].getTeamOneScore() > matches[i].getTeamTwoScore())
            ++teamOneWins;
        else if (matches[i].getTeamTwoScore() > matches[i].getTeamOneScore())
            ++teamTwoWins;
        else
            ++ties;

        totalPoints += matches[i].getTeamOneScore() + matches[i].getTeamTwoScore();
    }

    cout << "\n========== COMPETITION SUMMARY ==========\n";
    cout << "Matches recorded       : " << count << '\n';
    cout << "Team 1-side wins       : " << teamOneWins << '\n';
    cout << "Team 2-side wins       : " << teamTwoWins << '\n';
    cout << "Tied matches           : " << ties << '\n';
    cout << "Total points awarded   : " << totalPoints << '\n';
    cout << "Object records created : " << DebateMatch::getTotalMatches() << '\n';
    cout << "\nNote: Team 1-side / Team 2-side wins are match-side counts,\n";
    cout << "not unique team standings across the whole competition.\n";
}

void showMenu() {
    cout << "\n============================================\n";
    cout << "     DEBATE COMPETITION DRAW & SCORING\n";
    cout << "============================================\n";
    cout << "1. Create debate draw and enter scores\n";
    cout << "2. View all debate matches\n";
    cout << "3. Search match by ID\n";
    cout << "4. View competition summary\n";
    cout << "0. Exit\n";
    cout << "--------------------------------------------\n";
}

int main() {
    DebateMatch matches[MAX_MATCHES]; // Array of objects.
    int matchCount = 0;
    int choice;

    cout << "Welcome to Debate Competition Draw and Scoring.\n";

    do {
        showMenu();
        choice = readInteger("Enter your choice: ", 0, 4);

        switch (choice) {
            case 1: createDrawAndScore(matches, matchCount); break;
            case 2: viewAllMatches(matches, matchCount); break;
            case 3: searchMatch(matches, matchCount); break;
            case 4: showCompetitionSummary(matches, matchCount); break;
            case 0: cout << "\nThank you. Goodbye!\n"; break;
            default: cout << "Invalid menu choice.\n";
        }
    } while (choice != 0);

    return 0;
}
