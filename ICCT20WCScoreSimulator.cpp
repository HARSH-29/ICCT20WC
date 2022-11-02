/*

ICC T20 world cup score simulator
Author: Harsh Gupta

Input: 
1. Give teams in a single group
E.g "
    IND
    BAN
    PAK
    SA
    NED
    ZIM
    "
    
2. Give matches played already with results
    "
    Format: <team1> <team2> <result: winner or "TIE">
    IND PAK IND
    IND SA SA
    SA NED TIE
    "
    
The program will simulate all the possibilities in the remaining matches
and give a final table containing final points of all the teams in the group
No explicit ordering etc. is guaranteed for the output
*/

#include<bits/stdc++.h>
using namespace std ;

class Team {
    public:
    string name;
    Team() {}
    Team(string name): name(name) {}
};

class Result {
    public:
    Team* winner;
    
    // If tie is set, winner is completely ignored.
    bool tie;
    
    Result() {
        winner = NULL;
        tie = false;
    }
    
    Result(Team* team, bool tie) {
        winner = team;
        tie = tie;
    }
    
    Result(bool tie) {
        tie = tie;
        winner = NULL;
    }
};

class Match {
    public:
    Team* team1;
    Team* team2;
    Result* result;
    
    Match() {
        team1 = NULL;
        team2 = NULL;
        result = NULL;
    }
    
    Match(Team* team1, Team* team2, Result* result) {
        this->team1 = team1;
        this->team2 = team2;
        this->result = result;
    }
    
    Match(Team* team1, Team* team2) {
        this->team1 = team1;
        this->team2 = team2;
        this->result = NULL;
    }
};

vector<Team*> teams;
vector<Match*> completedMatches;

class SimulationTable {
    public:
    vector<Match> matches;
    
    bool isSame(Match match1, Match match2) {
        if (match1.team1->name == match2.team1->name) {
            return match1.team2->name == match2.team2->name;
        }
        if (match1.team1->name == match2.team2->name) {
            return match1.team2->name == match2.team1->name;
        }
        return false;
    }
    
    bool isExactlySameMatch(Match match1, Match match2) {
        return isSame(match1, match2) && match1.result->tie == match2.result->tie 
            && (
                (match1.result->winner == NULL && match2.result->winner == NULL) 
                || 
                (match1.result->winner->name == match2.result->winner->name));
    }
    
    bool isMatchAlreadyPlayed(Match match) {
        for ( int i = 0 ; i < matches.size() ; i++ ) {
            if (isSame(matches[i], match)) return true ;
        }
        return false;
    }
    
    bool isValidMatch(Match match) {
        if (match.team1->name == match.team2->name) return false; 
        return true;
    }
    
    void addMatch(Match match) {
        if (!isValidMatch(match)) {
            cout << "WHAT ARE YOU DOING HARSH?\n";
            return;
        }
        if (isMatchAlreadyPlayed(match)) {
            cout << "ARE YOU MAD?\n" ;
            return;
        }
        matches.push_back(match);
    }
    
    bool isMatchForTeam(Match match, Team team) {
        return match.team1->name == team.name || match.team2->name == team.name ;
    }
    
    int calculateTotalTeamPoints(Team team) {
        int ans = 0 ;
        for ( int i = 0 ; i < matches.size() ; i++ ) {
            if (isMatchForTeam(matches[i], team)) {
                if (matches[i].result->winner == NULL) ans += 1;
                else if (matches[i].result->winner->name == team.name) ans += 2;
            }
        }
        return ans ;
    }
    
    void printSimulatedTable() {
        cout << "**** HAHA NEW TABLE FOUND ******\n";
        for ( int i = 0 ; i < teams.size() ; i++ ) {
            cout << "Team: " << teams[i]->name << " " << " Points: " << calculateTotalTeamPoints(*teams[i]) << endl ;
        }
    }
    
    int calculateTotalTeamMatches(Team team) {
        int ans = 0 ;
        for ( int i = 0 ; i < matches.size() ; i++ ) {
            if (isMatchForTeam(matches[i], team)) ans++ ;   
        }
        return ans ;
    }
    
    bool allMatchesDone() {
        for ( int i = 0 ; i < teams.size() ; i++ ) {
            if (calculateTotalTeamMatches(*teams[i]) != teams.size() - 1) return false; 
        }
        return true;
    }
    
    void removeMatch(Match match) {
        vector<Match> newMatches;
        for ( int i = 0 ; i < matches.size() ; i++ ) {
            if (isSame(matches[i], match)) continue ;
            newMatches.push_back(matches[i]);
        }
        matches = newMatches;
    }
    
    bool matchPlayedBetween(Team team1, Team team2) {
        for ( int i = 0 ; i < matches.size() ; i++ ) {
            if (isMatchForTeam(matches[i], team1) && isMatchForTeam(matches[i], team2)) return true ;
        }
        return false;
    }
};

Team* getTeamByName(string teamName) {
    for(int i = 0 ; i < teams.size() ; i++ ) {
        if (teams[i]->name == teamName) return teams[i] ;
    }
    cout << "WHAAAA... no team found with name " << teamName << endl ;
    return teams[0];
}

Match* getRemainingMatch(SimulationTable simulationTable) {
    for ( int i = 0 ; i < teams.size() ; i++ ) {
        for ( int j = 0 ; j < teams.size() ; j++ ) {
            if ( i == j ) continue ;
            if (!simulationTable.matchPlayedBetween(*teams[i], *teams[j])) {
                Match* match = new Match(teams[i], teams[j]);
                return match;
            }
        }
    }
    cout << "ALL MATCHES ARE PLAYED HARSH, WTF DID YOU CODE?\n";
    Match* match = new Match(teams[0], teams[0]);
    return match;
}

vector<Result> getPossibleResults(Match match) {
    return {
        Result(match.team1, false),
        Result(match.team2, false),
        Result(true)
    };
}

void printMatch(Match* match) {
    cout << match->team1->name << " " << match->team2->name << " " ;
    if (match->result->winner == NULL) {
        cout << "TIED";
    } else {
        cout << match->result->winner->name;
    }
    cout << endl;
}

void simulateMatches(SimulationTable simulationTable) {
    if (simulationTable.allMatchesDone()) {
        simulationTable.printSimulatedTable();
        return;
    }
    
    // Get the reaming match between teams
    Match* remaining = getRemainingMatch(simulationTable);
    
    for(auto result: getPossibleResults(*remaining)) {
        remaining->result = &result;
        printMatch(remaining);
        simulationTable.addMatch(*remaining);
        simulateMatches(simulationTable);
        simulationTable.removeMatch(*remaining);
    }
}

void runSimulations() {
    SimulationTable simulationTable;
    for ( int i = 0 ; i < completedMatches.size() ; i++ ) {
        simulationTable.addMatch(*completedMatches[i]);
    }
    simulateMatches(simulationTable);
}

int main () {
    string input;
    
    cout << "Enter teams in a group, one in each line\n";
    cout << "Enter DONE when completed\n";
    while(true) {
        cin >> input;
        if (input == "DONE") break;
        Team* team = new Team();
        team->name = input;
        teams.push_back(team);
    }
    
    cout << "Enter already played matches with results, one in each line\n";
    cout << "Enter DONE when completed\n";
    while(true) {
        cin >> input;
        if (input == "DONE") break ;
        string team1String = "", team2String = "", resultString = "";
        
        int counter = 0 ;
        for ( int i = 0 ; i < input.size() ; i++ ) {
            if ( input[i] == '-' ) {
                counter++ ;
                continue;
            }
            if ( counter == 0 ) team1String.push_back(input[i]);
            else if (counter == 1) team2String.push_back(input[i]);
            else resultString.push_back(input[i]);
        }
        Team* team1 = getTeamByName(team1String);
        Team* team2 = getTeamByName(team2String);
        Match* match = new Match();
        match->team1 = team1;
        match->team2 = team2;
        
        if (resultString == "TIE") {
            Result* result = new Result(true);
            match->result = result;
            completedMatches.push_back(match);
        } else {
            Team* winningTeam = getTeamByName(resultString);
            Result* result = new Result(winningTeam, false);
            match->result = result;
            completedMatches.push_back(match);   
        }
    }
    
    
    cout << "Running all possible simulations....\n";
    cout << "Why don't you take a cup of tea till then......\n";
    
    runSimulations();
}
