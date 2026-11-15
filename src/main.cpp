#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <cstring>
using namespace std;
const char DATA_FILE[]="data/debate_matches.dat";
const int MAX_SCORE=100;

struct DebateMatchRecord {
    char matchId[24], roundName[48], teamOne[64], teamTwo[64], debateMotion[200];
    int teamOneScore, teamTwoScore;
};

class DebateMatch {
    string matchId, roundName, teamOne, teamTwo, debateMotion;
    int teamOneScore, teamTwoScore;
    static int totalMatches, liveObjects;
    static void copyText(char* out,size_t cap,const string& in) {
        size_t n=in.size(); if(n>=cap)n=cap-1; memcpy(out,in.c_str(),n); out[n]='\0';
    }
public:
    DebateMatch():matchId(""),roundName(""),teamOne(""),teamTwo(""),debateMotion(""),teamOneScore(0),teamTwoScore(0){++liveObjects;}
    DebateMatch(const string& id,const string& round,const string& a,const string& b,const string& motion,int sa,int sb)
      :matchId(id),roundName(round),teamOne(a),teamTwo(b),debateMotion(motion),teamOneScore(sa),teamTwoScore(sb){++totalMatches;++liveObjects;}
    DebateMatch(const DebateMatch& x):matchId(x.matchId),roundName(x.roundName),teamOne(x.teamOne),teamTwo(x.teamTwo),debateMotion(x.debateMotion),teamOneScore(x.teamOneScore),teamTwoScore(x.teamTwoScore){++totalMatches;++liveObjects;}
    ~DebateMatch(){--liveObjects;}
    void setDetails(const string& id,const string& round,const string& a,const string& b,const string& motion,int sa,int sb){
        this->matchId=id;this->roundName=round;this->teamOne=a;this->teamTwo=b;this->debateMotion=motion;this->teamOneScore=sa;this->teamTwoScore=sb;
    }
    inline string getMatchId()const{return matchId;}
    inline string getRoundName()const{return roundName;}
    inline string getTeamOne()const{return teamOne;}
    inline string getTeamTwo()const{return teamTwo;}
    inline int getTeamOneScore()const{return teamOneScore;}
    inline int getTeamTwoScore()const{return teamTwoScore;}
    string getWinner()const {if(teamOneScore>teamTwoScore)return teamOne;if(teamTwoScore>teamOneScore)return teamTwo;return "Tie";}
    DebateMatchRecord toRecord()const {
        DebateMatchRecord r={};copyText(r.matchId,sizeof(r.matchId),matchId);copyText(r.roundName,sizeof(r.roundName),roundName);
        copyText(r.teamOne,sizeof(r.teamOne),teamOne);copyText(r.teamTwo,sizeof(r.teamTwo),teamTwo);copyText(r.debateMotion,sizeof(r.debateMotion),debateMotion);
        r.teamOneScore=teamOneScore;r.teamTwoScore=teamTwoScore;return r;
    }
    static DebateMatch fromRecord(const DebateMatchRecord&r){return DebateMatch(r.matchId,r.roundName,r.teamOne,r.teamTwo,r.debateMotion,r.teamOneScore,r.teamTwoScore);}
    static int getTotalMatches(){return totalMatches;}
    static int getLiveObjects(){return liveObjects;}
    friend void displayMatch(const DebateMatch&);
};
int DebateMatch::totalMatches=0; int DebateMatch::liveObjects=0;

void displayMatch(const DebateMatch&m){
    cout<<"\nMatch ID: "<<m.matchId<<"\nRound: "<<m.roundName<<"\nTeam 1: "<<m.teamOne<<"\nTeam 2: "<<m.teamTwo
        <<"\nMotion: "<<m.debateMotion<<"\nScore: "<<m.teamOneScore<<" - "<<m.teamTwoScore<<"\nResult: "<<m.getWinner()<<"\n";
}
string textInput(const string&p){string s;do{cout<<p;getline(cin,s);bool ok=false;for(char c:s)if(!isspace((unsigned char)c)){ok=true;break;}if(ok)return s;cout<<"Value cannot be blank.\n";}while(true);}
int numberInput(const string&p,int lo,int hi){int n;while(true){cout<<p;if(cin>>n&&n>=lo&&n<=hi){cin.ignore(numeric_limits<streamsize>::max(),'\n');return n;}cout<<"Enter a whole number from "<<lo<<" to "<<hi<<".\n";cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');}}
bool ensureFile(){ifstream in(DATA_FILE,ios::binary);if(in.good())return true;in.close();ofstream out(DATA_FILE,ios::binary);return out.good();}
bool validLengths(const string&id,const string&r,const string&a,const string&b,const string&m){
 return id.size()<sizeof(DebateMatchRecord::matchId)&&r.size()<sizeof(DebateMatchRecord::roundName)&&a.size()<sizeof(DebateMatchRecord::teamOne)&&b.size()<sizeof(DebateMatchRecord::teamTwo)&&m.size()<sizeof(DebateMatchRecord::debateMotion);
}
bool readAt(fstream&f,streamoff pos,DebateMatchRecord&r){f.clear();f.seekg(pos,ios::beg);if(!f)return false;f.read(reinterpret_cast<char*>(&r),sizeof(r));return f.gcount()==sizeof(r);}
streamoff locate(fstream&f,const string&id){
 f.clear();f.seekg(0,ios::end);streamoff size=f.tellg();if(size<0||size%sizeof(DebateMatchRecord))return -1;
 for(streamoff p=0;p<size;p+=sizeof(DebateMatchRecord)){DebateMatchRecord r={};if(!readAt(f,p,r))return -1;if(id==r.matchId)return p;}return -1;
}
bool teamUsed(fstream&f,const string&round,const string&team){
 f.clear();f.seekg(0,ios::end);streamoff size=f.tellg();if(size<0||size%sizeof(DebateMatchRecord))return false;
 for(streamoff p=0;p<size;p+=sizeof(DebateMatchRecord)){DebateMatchRecord r={};if(!readAt(f,p,r))return false;if(round==r.roundName&&(team==r.teamOne||team==r.teamTwo))return true;}return false;
}
void addMatch(){
 string id=textInput("Match ID: "),round=textInput("Round: "),a=textInput("Team 1: "),b=textInput("Team 2: "),motion=textInput("Debate motion: ");
 if(!validLengths(id,round,a,b,motion)){cout<<"Text exceeds fixed binary field capacity. Record rejected.\n";return;}
 if(a==b){cout<<"A team cannot debate against itself.\n";return;}
 int sa=numberInput("Team 1 score (0-100): ",0,100),sb=numberInput("Team 2 score (0-100): ",0,100);
 fstream f(DATA_FILE,ios::in|ios::out|ios::binary);if(!f){cout<<"Cannot open data file.\n";return;}
 if(locate(f,id)!=-1){cout<<"Duplicate match ID rejected.\n";return;}
 if(teamUsed(f,round,a)||teamUsed(f,round,b)){cout<<"Team already assigned in this round.\n";return;}
 DebateMatch m(id,round,a,b,motion,sa,sb); DebateMatch copy(m); DebateMatchRecord rec=copy.toRecord();
 f.clear();f.seekp(0,ios::end);f.write(reinterpret_cast<const char*>(&rec),sizeof(rec));f.flush();
 if(f.good())cout<<"Match saved permanently. Winner/result: "<<m.getWinner()<<"\n";else cout<<"Write failed.\n";
}
void viewAll(){ifstream f(DATA_FILE,ios::binary);DebateMatchRecord r={};int n=0;while(f.read(reinterpret_cast<char*>(&r),sizeof(r))){DebateMatch m=DebateMatch::fromRecord(r);displayMatch(m);++n;}if(f.gcount()!=0)cout<<"Warning: incomplete trailing record.\n";if(!n)cout<<"No saved matches.\n";else cout<<"Total matches: "<<n<<"\n";}
void searchMatch(){string id=textInput("Match ID to search: ");fstream f(DATA_FILE,ios::in|ios::binary);streamoff p=locate(f,id);if(p<0){cout<<"Match not found.\n";return;}DebateMatchRecord r={};if(readAt(f,p,r)){DebateMatch m=DebateMatch::fromRecord(r);displayMatch(m);}}
void updateMatch(){string id=textInput("Match ID to update: ");fstream f(DATA_FILE,ios::in|ios::out|ios::binary);if(!f){cout<<"Cannot open data file.\n";return;}streamoff p=locate(f,id);if(p<0){cout<<"Match not found.\n";return;}DebateMatchRecord r={};if(!readAt(f,p,r)){cout<<"Read failed.\n";return;}
 cout<<"Current: "<<r.teamOne<<" "<<r.teamOneScore<<" - "<<r.teamTwoScore<<" "<<r.teamTwo<<"\n";
 int a=numberInput("New Team 1 score: ",0,100),b=numberInput("New Team 2 score: ",0,100),yes=numberInput("Confirm update (1=yes, 0=no): ",0,1);
 if(!yes){cout<<"Update cancelled.\n";return;}r.teamOneScore=a;r.teamTwoScore=b;f.clear();f.seekp(p,ios::beg);f.write(reinterpret_cast<char*>(&r),sizeof(r));f.flush();cout<<(f.good()?"Scores updated using seekp().\n":"Update failed.\n");
}
void summary(){ifstream f(DATA_FILE,ios::binary);DebateMatchRecord r={};int n=0,a=0,b=0,t=0,points=0;while(f.read(reinterpret_cast<char*>(&r),sizeof(r))){++n;if(r.teamOneScore>r.teamTwoScore)++a;else if(r.teamTwoScore>r.teamOneScore)++b;else ++t;points+=r.teamOneScore+r.teamTwoScore;}cout<<"Matches: "<<n<<"\nTeam 1-side wins: "<<a<<"\nTeam 2-side wins: "<<b<<"\nTies: "<<t<<"\nTotal points: "<<points<<"\n";}
int main(){
 if(!ensureFile()){cerr<<"Cannot initialise "<<DATA_FILE<<". Ensure data directory exists.\n";return 1;}
 int c;cout<<"Debate Competition Draw and Scoring — CA2\nBinary file: "<<DATA_FILE<<"\n";
 do{cout<<"\n1 Add match\n2 View all\n3 Search by ID (seekg)\n4 Update scores (seekp)\n5 Summary\n0 Exit\n";c=numberInput("Choice: ",0,5);
 switch(c){case 1:addMatch();break;case 2:viewAll();break;case 3:searchMatch();break;case 4:updateMatch();break;case 5:summary();break;case 0:cout<<"Records remain saved after exit.\n";break;}}while(c!=0);
 return 0;
}
