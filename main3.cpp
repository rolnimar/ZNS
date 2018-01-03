#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <set>
#include <ncurses.h>

using namespace std;


//-------------------------------------QUESTIONS---------------------------------------------------
class Question{
public:
    Question(){};
    Question(const string&, const string&);
    string getQuestion();
    string getJoin();
private:
    string question;
    string join;
};

Question::Question(const string & x, const string & y) {
    this->question = x;
    this->join = y;
}

string Question::getQuestion() {
    return question;
}

string Question::getJoin() {
    return join;
}

class Questions {
public:
    Questions(){};
    void AddQuestion(Question);
    int getLength();
    string getQuestion(int);
    string getJoin(int);
    vector <Question> getQuestions();
private:
    vector<Question> questions;
};

vector <Question> Questions::getQuestions() {
    return questions;
}

void Questions::AddQuestion(Question x) {
    questions.push_back(x);
}

int Questions::getLength() {
    return questions.size();
}

string Questions::getQuestion(int index) {
    return questions[index].getQuestion();
}

string Questions::getJoin(int index) {
    return questions[index].getJoin();
}


//-------------------------------------CONCLUSIONS---------------------------------------------------
class Fuzzy{
public:
    Fuzzy(){};
    Fuzzy(string x, double y):join(x),value(y){

    }

    double getValue();
    string getJoin();
private:
    string join;
    double value;
};

string Fuzzy::getJoin() {
    return join;
}

double Fuzzy::getValue() {
    return value;
}


class Conclusion{
public:
    Conclusion( string,vector <Fuzzy>);
    vector <Fuzzy> getFuzzy() const;
    string getConclusion();
private:
    string conclusion;
    vector <Fuzzy> fuzzyvalue;
};

string Conclusion::getConclusion() {
    return conclusion;
}
Conclusion::Conclusion(string x, vector <Fuzzy> y) {
    this->conclusion = x;
    this->fuzzyvalue = y;
}

vector <Fuzzy> Conclusion::getFuzzy() const {
    return fuzzyvalue;
}

class Conclusions{
public:
    Conclusions(){};
    void AddConclusion(const Conclusion);
    vector <Conclusion> getConclusions();
    string getConclusion(int);

private:
    vector <Conclusion> conclusions;
};

string Conclusions::getConclusion(int index) {
    return conclusions[index].getConclusion();
}

vector <Conclusion> Conclusions::getConclusions() {
    return conclusions;
}

void Conclusions::AddConclusion(const Conclusion x) {
    conclusions.push_back(x);
}



//-------------------------------------GAME---------------------------------------------------

class Game{
public:
    Game(){};
    ~Game(){};
    void StartGame();
    void AddQuestions( Questions, string);
    void AddConclusions(Conclusions,string);

private:
    Questions questionPoolPC;
    Questions questionPoolPS;
    Conclusions conclusionPoolPC;
    Conclusions conclusionPoolPS;
    string input;
    void getInput();
    vector <double> valuesOld;
    vector <double> valuesNew;
    int randNum;
    set <int> questionsAsked;
    vector <double> compare(vector <double>, vector <double>);

};
vector <double> Game::compare(vector<double> x, vector<double> y) {
    for(int i = 0; i < x.size();i++){
        x[i] = min(x[i],y[i]);
    }
    return x;
}

void Game::AddQuestions(Questions x, string y) {
    if(y=="PC"){
        this->questionPoolPC=x;
        return;
    }
    if(y=="PS"){
        this->questionPoolPS = x;
        return;
    }

}


void Game::AddConclusions(Conclusions x, string y) {
    if( y =="PC"){
        this->conclusionPoolPC = x;
        return;
    }
    if( y=="PS"){
        this->conclusionPoolPS = x;
        return;
    }
}

void Game::getInput() {
    cin >> input;
}


void Game::StartGame() {
    initscr();
    noecho();
    cbreak();



    int maxY, maxX;
    getmaxyx(stdscr,maxY,maxX);

    WINDOW* menu1win = newwin(20, maxX-12, maxY-30, 5 );
    WINDOW* question = newwin(5,maxX-12, maxY-40, 5);
    WINDOW* conclusion = newwin(35, maxX-100, maxY-50,50);
    box(question,0,0);
    box(menu1win, 0, 0);
    refresh();
    wrefresh(menu1win);
    wrefresh(question);
    keypad(menu1win,true);
    curs_set(0);

    string choices1[2] = {"YES","NO"};
    int choice;
    int highlight = 0;

    string str = "Do you want to play a game?";

    wattron(question,A_BOLD);
    mvwprintw(question,2,maxX/2-floor((str.size())/2)-5,str.c_str());
    wrefresh(question);
    wattroff(question,A_BOLD);

    while(1){
        for (int i = 0 ; i < 2 ; i++){
            if(i == highlight)
                wattron(menu1win,A_REVERSE);
            mvwprintw(menu1win,10, maxX/4+i*maxX/2-10,choices1[i].c_str());

            wattroff(menu1win, A_REVERSE);

        }

        choice = wgetch(menu1win);


        switch (choice){
            case KEY_LEFT:
                highlight--;
                if(highlight == -1)
                    highlight =0;
                break;
            case KEY_RIGHT:
                highlight++;
                if(highlight==2)
                    highlight=1;
                break;
            default:
                break;
        }
        if(choice == 10)
            break;
    }

    getch();





    if(highlight == 1) {
        wclear(question);
        wclear(menu1win);
        box(conclusion,0,0);
        str = "Go play chess then..";
        mvwprintw(conclusion,5,maxX/2-(str.size()/2)-50,str.c_str());
        wrefresh(conclusion);
        getch();
        endwin();
        return;
    }else {
        str = "Do yo uwant to play a PS game?";
        wclear(question);
        box(question, 0, 0);
        wattron(question, A_BOLD);
        mvwprintw(question, 2, maxX / 2 - floor((str.size()) / 2) - 5, str.c_str());
        wrefresh(question);
        wattroff(question, A_BOLD);
    }



        if(input == "no"){
            cout << "Do you want to play a PC game?" << endl;
            getInput();
            if(input =="no"){
                cout << "Go play chess then..." << endl;
            } else {
                for(int i = 0; i< conclusionPoolPC.getConclusions().size();i++){
                    valuesOld.push_back(1);
                }
                //PC GAMES------------------------------------------------------------
                while(1) {
                    srand(time(NULL));
                    randNum = rand() % (questionPoolPC.getLength());
                    while(1){
                        cout << randNum << endl;
                        auto it = questionsAsked.find(randNum);
                        if(it == questionsAsked.end()){
                            questionsAsked.insert(randNum);
                            break;
                        } else{
                            randNum+=1;
                            randNum%=questionPoolPC.getLength();
                        }


                    }
                    double maxvalue=0;
                    int maxindex=0;

                    if(questionPoolPC.getLength()==questionsAsked.size()){
                        for(int i = 0; i < valuesOld.size();i++){
                            if(valuesOld[i]>=maxvalue){
                                maxvalue=valuesOld[i];
                                maxindex = i;
                            }
                        }
                        cout << "Your game is: " << endl << conclusionPoolPC.getConclusion(maxindex)<<endl;
                        return;
                    }

                    cout << questionPoolPC.getQuestion(randNum) << endl;
                    getInput();

                    for(int i = 0; i < conclusionPoolPC.getConclusions().size();i++){
                        for(int j = 0; j< conclusionPoolPC.getConclusions()[i].getFuzzy().size();j++){
                            if(conclusionPoolPC.getConclusions()[i].getFuzzy()[j].getJoin() == questionPoolPC.getJoin(randNum)){
                                valuesNew.push_back(conclusionPoolPC.getConclusions()[i].getFuzzy()[j].getValue());
                            }
                        }
                    }
                    cout << "values new1"<< endl;
                    for(int i = 0; i < valuesNew.size();i++) {
                        cout << valuesNew[i] << " ";
                    }
                    cout << endl;
                    cout << "values old1"<<endl;
                    for(int i = 0; i < valuesOld.size();i++) {
                        cout << valuesOld[i] << " ";
                    }
                    cout << endl;


                    if(input=="no"){
                        for(int i = 0; i < valuesNew.size();i++){
                            valuesNew[i]=1-valuesNew[i];
                        }
                        valuesOld = compare(valuesOld,valuesNew);
                    } else {
                        valuesOld = compare(valuesOld,valuesNew);
                    }


                    int differentToZero = 0;
                    for(int i = 0; i < valuesOld.size();i++){
                        if(valuesOld[i]!=0){
                            differentToZero++;
                        }
                        if(valuesOld[i]<0.3){
                            valuesOld[i]=0;
                        }
                    }
                    cout << "values new2"<< endl;
                    for(int i = 0; i < valuesNew.size();i++) {
                        cout << valuesNew[i] << " ";
                    }
                    cout << endl;
                    cout << "values old2"<<endl;
                    for(int i = 0; i < valuesOld.size();i++) {
                        cout << valuesOld[i] << " ";
                    }

                    maxvalue = 0;
                    maxindex = 0;
                    if(differentToZero == 0){
                        cout << "Sorry, it was impossible to determine the game you want to play. Try again."<<endl;
                        return;
                    }
                    if(differentToZero<3){
                        for(int i = 0; i < valuesOld.size();i++){
                            if(valuesOld[i]>=maxvalue){
                                maxvalue=valuesOld[i];
                                maxindex = i;
                            }
                        }
                        cout << "Your game is: " << endl << conclusionPoolPC.getConclusion(maxindex)<<endl;
                        return;
                    }

                    valuesNew.clear();
                }




            }
        } else{
            // PS GAMES----------------------------------------------------
            for(int i = 0; i< conclusionPoolPS.getConclusions().size();i++){
                valuesOld.push_back(1);
            }

            while(1) {
                srand(time(NULL));
                randNum = rand() % (questionPoolPS.getLength());
                while(1){
                    cout << randNum << endl;
                    auto it = questionsAsked.find(randNum);
                    if(it == questionsAsked.end()){
                        questionsAsked.insert(randNum);
                        break;
                    } else{
                        randNum+=1;
                        randNum%=questionPoolPS.getLength();
                    }


                }
                double maxvalue=0;
                int maxindex=0;

                if(questionPoolPS.getLength()==questionsAsked.size()){
                    for(int i = 0; i < valuesOld.size();i++){
                        if(valuesOld[i]>=maxvalue){
                            maxvalue=valuesOld[i];
                            maxindex = i;
                        }
                    }
                    cout << "Your game is: " << endl << conclusionPoolPS.getConclusion(maxindex)<<endl;
                    return;
                }

                cout << questionPoolPS.getQuestion(randNum) << endl;
                getInput();

                for(int i = 0; i < conclusionPoolPS.getConclusions().size();i++){
                    for(int j = 0; j< conclusionPoolPS.getConclusions()[i].getFuzzy().size();j++){
                        if(conclusionPoolPS.getConclusions()[i].getFuzzy()[j].getJoin() == questionPoolPS.getJoin(randNum)){
                            valuesNew.push_back(conclusionPoolPS.getConclusions()[i].getFuzzy()[j].getValue());
                        }
                    }
                }
                cout << "values new1"<< endl;
                for(int i = 0; i < valuesNew.size();i++) {
                    cout << valuesNew[i] << " ";
                }
                cout << endl;
                cout << "values old1"<<endl;
                for(int i = 0; i < valuesOld.size();i++) {
                    cout << valuesOld[i] << " ";
                }
                cout << endl;


                if(input=="no"){
                    for(int i = 0; i < valuesNew.size();i++){
                        valuesNew[i]=1-valuesNew[i];
                    }
                    valuesOld = compare(valuesOld,valuesNew);
                } else {
                    valuesOld = compare(valuesOld,valuesNew);
                }


                int differentToZero = 0;
                for(int i = 0; i < valuesOld.size();i++){
                    if(valuesOld[i]!=0){
                        differentToZero++;
                    }
                    if(valuesOld[i]<0.3){
                        valuesOld[i]=0;
                    }
                }
                cout << "values new2"<< endl;
                for(int i = 0; i < valuesNew.size();i++) {
                    cout << valuesNew[i] << " ";
                }
                cout << endl;
                cout << "values old2"<<endl;
                for(int i = 0; i < valuesOld.size();i++) {
                    cout << valuesOld[i] << " ";
                }

                maxvalue = 0;
                maxindex = 0;
                if(differentToZero == 0){
                    cout << "Sorry, it was impossible to determine the game you want to play. Try again."<<endl;
                    return;
                }

                if(differentToZero<3){
                    for(int i = 0; i < valuesOld.size();i++){
                        if(valuesOld[i]>=maxvalue){
                            maxvalue=valuesOld[i];
                            maxindex = i;
                        }
                    }
                    cout << "Your game is: " << endl << conclusionPoolPS.getConclusion(maxindex)<<endl;
                    return;
                }

                valuesNew.clear();
            }




        }
        }
*/
    getch();
    endwin();
    }






//-------------------------------------MAIN---------------------------------------------------

int main(){

    Questions questionPoolPC = Questions();
    Questions questionPoolPS = Questions();




    questionPoolPC.AddQuestion(Question("Do you want to play a challenging game?", "challenging"));
    questionPoolPC.AddQuestion(Question("Do you want to play a star wars game?", "starwars"));
    questionPoolPC.AddQuestion(Question("Do you want to experience a good story in a game?", "story"));
    questionPoolPC.AddQuestion(Question("Do you want to explore a lot?", "explore"));
    questionPoolPC.AddQuestion(Question("Do you want to play a male character?", "male"));
    questionPoolPC.AddQuestion(Question("Do you want to play a czech game?", "czech"));
    questionPoolPC.AddQuestion(Question("Do you want to play a 2D game?", "2D"));
    questionPoolPC.AddQuestion(Question("Do you want to play PvP?", "PVP"));
    questionPoolPC.AddQuestion(Question("Do you want to play a shooter?", "shooter"));
    questionPoolPC.AddQuestion(Question("Do you want to go through portals?", "portals"));
    questionPoolPC.AddQuestion(Question("Do you want to be a gangster?", "gangster"));
    questionPoolPC.AddQuestion(Question("Do you want to play a horror game?", "horror"));
    questionPoolPC.AddQuestion(Question("Do you never want the game to end when you finish playing it?", "end"));
    questionPoolPC.AddQuestion(Question("Do you want to play an adventure game?", "adventure"));
    questionPoolPC.AddQuestion(Question("Do you want to use a lightsaber?", "lightsaber"));
    questionPoolPC.AddQuestion(Question("Do you want shooting to be in the game?", "shooting"));
    questionPoolPC.AddQuestion(Question("Do you want to be a part of WW2?", "WW2"));
    questionPoolPC.AddQuestion(Question("Do you want to play a racing game?", "racing"));
    questionPoolPC.AddQuestion(Question("Do you want to tune your car?", "tuning"));
    questionPoolPC.AddQuestion(Question("Do you want to play online?", "online"));
    questionPoolPC.AddQuestion(Question("Do you want to play a MMORPG?", "MMORPG"));
    questionPoolPC.AddQuestion(Question("Do you like playing against a bunch of children?", "children"));
    questionPoolPC.AddQuestion(Question("Do you want to play a card game?", "cards"));
    questionPoolPC.AddQuestion(Question("Do you want to play an oldschool game?", "oldschool"));
    questionPoolPC.AddQuestion(Question("Do you want to play a game with good graphics??", "graphics"));
    questionPoolPC.AddQuestion(Question("Do you want to play a game with a lot of jokes?", "jokes"));
    questionPoolPC.AddQuestion(Question("Do you want to play a super chill game and want to have fun?", "chill"));
    questionPoolPC.AddQuestion(Question("Do you want to unlock a lot of stuff in the game?", "unlock"));




    questionPoolPS.AddQuestion(Question("Do you want to play a challenging game?", "challenging"));
    questionPoolPS.AddQuestion(Question("Do you want to play a Star Wars?", "starwars"));
    questionPoolPS.AddQuestion(Question("Do you want to experience a good story in a game?", "story"));
    questionPoolPS.AddQuestion(Question("Do you want to explore and jump a lot?", "explore"));
    questionPoolPS.AddQuestion(Question("Do you want to play a male character?", "male"));
    questionPoolPS.AddQuestion(Question("Do you want to play a 2D game?", "2D"));
    questionPoolPS.AddQuestion(Question("Do you want to play PvP?", "PVP"));
    questionPoolPS.AddQuestion(Question("Do you want to play a shooter?", "shooter"));
    questionPoolPS.AddQuestion(Question("Do you want to go through portals?", "portals"));
    questionPoolPS.AddQuestion(Question("Do you want to play a lego game?", "lego"));
    questionPoolPS.AddQuestion(Question("Do you want to play a horror game?", "horror"));
    questionPoolPS.AddQuestion(Question("Do you never want the game to end when you finish playing it?", "end"));
    questionPoolPS.AddQuestion(Question("Do you want to use a lightsaber?", "lightsaber"));
    questionPoolPS.AddQuestion(Question("Do you want shooting to be in the game?", "shooting"));
    questionPoolPS.AddQuestion(Question("Do you want to play a racing game?", "racing"));
    questionPoolPS.AddQuestion(Question("Do you want to tune your car?", "tuning"));
    questionPoolPS.AddQuestion(Question("Do you want to play online?", "online"));
    questionPoolPS.AddQuestion(Question("Do you want to play a game with good graphics??", "graphics"));
    questionPoolPS.AddQuestion(Question("Do you want to play a super chill game and want to have fun?", "chill"));
    questionPoolPS.AddQuestion(Question("Do you want to unlock a lot of stuff in the game?", "unlock"));


    Conclusions conclusionPoolPC = Conclusions();
    Conclusions conclusionPoolPS = Conclusions();

    vector <Fuzzy> values;

    values.push_back(Fuzzy("challenging",0.9));
    values.push_back(Fuzzy("oldschool",0.8));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.1));
    values.push_back(Fuzzy("explore",0.8));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",1));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.2));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.9));
    values.push_back(Fuzzy("adventure",0.6));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.8));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.4));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.8));
    values.push_back(Fuzzy("unlock",0.5));
    conclusionPoolPC.AddConclusion(Conclusion("SuperMeatBoy",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.9));
    values.push_back(Fuzzy("oldschool",0.8));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.1));
    values.push_back(Fuzzy("explore",0.8));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",1));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.2));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.9));
    values.push_back(Fuzzy("adventure",0.6));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.8));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.4));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.8));
    values.push_back(Fuzzy("unlock",0.5));
    conclusionPoolPS.AddConclusion(Conclusion("SuperMeatBoy",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.9));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.2));
    values.push_back(Fuzzy("explore",0.8));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.6));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.2));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.2));
    values.push_back(Fuzzy("end",0.6));
    values.push_back(Fuzzy("adventure",0.6));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.3));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPC.AddConclusion(Conclusion("Dark Souls, Nioh",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.9));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.2));
    values.push_back(Fuzzy("explore",0.8));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.6));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.2));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.2));
    values.push_back(Fuzzy("end",0.6));
    values.push_back(Fuzzy("adventure",0.6));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.3));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Dark Souls, Nioh",values));
    values.clear();





    values.push_back(Fuzzy("challenging",0.5));
    values.push_back(Fuzzy("oldschool",0.9));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",0.8));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",1));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0.9));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",1));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.1));
    values.push_back(Fuzzy("end",0.8));
    values.push_back(Fuzzy("adventure",0.4));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.8));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.6));
    values.push_back(Fuzzy("jokes",0.7));
    values.push_back(Fuzzy("chill",0.4));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPC.AddConclusion(Conclusion("Mafia",values));
    values.clear();





    values.push_back(Fuzzy("challenging",0.2));
    values.push_back(Fuzzy("oldschool",0.9));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",1));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.1));
    values.push_back(Fuzzy("shooter",0.9));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.1));
    values.push_back(Fuzzy("end",0.8));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.9));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.3));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.3));
    values.push_back(Fuzzy("jokes",0.7));
    values.push_back(Fuzzy("chill",0.2));
    values.push_back(Fuzzy("unlock",0.1));
    conclusionPoolPC.AddConclusion(Conclusion("Vietcong",values));
    values.clear();



    values.push_back(Fuzzy("challenging",0.9));
    values.push_back(Fuzzy("oldschool",1));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.6));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",1));
    values.push_back(Fuzzy("2D",1));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.1));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.2));
    values.push_back(Fuzzy("adventure",0.9));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.1));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0.3));
    values.push_back(Fuzzy("graphics",0.3));
    values.push_back(Fuzzy("jokes",0.5));
    values.push_back(Fuzzy("chill",0.6));
    values.push_back(Fuzzy("unlock",0));
    conclusionPoolPC.AddConclusion(Conclusion("Polda",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.4));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.3));
    values.push_back(Fuzzy("explore",1));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.8));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.1));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.2));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",1));
    values.push_back(Fuzzy("MMORPG",1));
    values.push_back(Fuzzy("children",0.3));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.2));
    values.push_back(Fuzzy("unlock",0.9));
    conclusionPoolPC.AddConclusion(Conclusion("World of Warcraft",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.6));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",1));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.1));
    values.push_back(Fuzzy("end",0));
    values.push_back(Fuzzy("adventure",0));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",1));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0.9));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.2));
    values.push_back(Fuzzy("chill",0.3));
    values.push_back(Fuzzy("unlock",1));
    conclusionPoolPC.AddConclusion(Conclusion("League of Legends",values));
    values.clear();




    values.push_back(Fuzzy("challenging",0.4));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0.8));
    values.push_back(Fuzzy("PVP",0.8));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",1));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0.1));
    values.push_back(Fuzzy("cards",1));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.3));
    values.push_back(Fuzzy("unlock",0.9));
    conclusionPoolPC.AddConclusion(Conclusion("Hearthstone",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.2));
    values.push_back(Fuzzy("oldschool",0.2));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",1));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.1));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",0.2));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",1));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0.2));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.6));
    conclusionPoolPC.AddConclusion(Conclusion("Counter Strike",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.2));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.1));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.1));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.1));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",1));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.3));
    conclusionPoolPC.AddConclusion(Conclusion("Crysis 1 & 2 & 3",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.3));
    values.push_back(Fuzzy("oldschool",1));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.9));
    values.push_back(Fuzzy("explore",0.1));
    values.push_back(Fuzzy("male",0.6));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",1));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0.3));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.4));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.8));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.4));
    values.push_back(Fuzzy("jokes",1));
    values.push_back(Fuzzy("chill",0.7));
    values.push_back(Fuzzy("unlock",0));
    conclusionPoolPC.AddConclusion(Conclusion("Undertale",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.2));
    values.push_back(Fuzzy("oldschool",0.6));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.7));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.7));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.2));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.3));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.5));
    values.push_back(Fuzzy("jokes",1));
    values.push_back(Fuzzy("chill",0.2));
    values.push_back(Fuzzy("unlock",0.2));
    conclusionPoolPC.AddConclusion(Conclusion("Max Payne",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.3));
    values.push_back(Fuzzy("oldschool",0.2));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.2));
    values.push_back(Fuzzy("explore",0.1));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.5));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0.9));
    values.push_back(Fuzzy("end",0.1));
    values.push_back(Fuzzy("adventure",0.2));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.5));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.1));
    conclusionPoolPC.AddConclusion(Conclusion("F.E.A.R",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.3));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.3));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",1));
    values.push_back(Fuzzy("end",0.1));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0));
    conclusionPoolPC.AddConclusion(Conclusion("Outlast",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.3));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.3));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",1));
    values.push_back(Fuzzy("end",0.1));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0));
    conclusionPoolPS.AddConclusion(Conclusion("Outlast",values));
    values.clear();



    values.push_back(Fuzzy("challenging",0.1));
    values.push_back(Fuzzy("oldschool",0.2));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.6));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.3));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.3));
    values.push_back(Fuzzy("adventure",0));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",1));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.4));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0.4));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.3));
    conclusionPoolPC.AddConclusion(Conclusion("Call of Duty WW2",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.7));
    values.push_back(Fuzzy("oldschool",0.2));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",0.7));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.9));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",1));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPC.AddConclusion(Conclusion("Ori and the blind forest \nBrothers: A Tale of Two Sons",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.7));
    values.push_back(Fuzzy("oldschool",0.2));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",0.7));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.9));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",1));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Ori and the blind forest \nBrothers: A Tale of Two Sons",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.4));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.5));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",1));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.5));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.5));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPC.AddConclusion(Conclusion("Need for Speed Shift\nForza",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.4));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.5));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",1));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.5));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0.5));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Need for Speed Shift\nForza",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.7));
    values.push_back(Fuzzy("oldschool",0.7));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.2));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.3));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",1));
    values.push_back(Fuzzy("tuning",1));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.6));
    values.push_back(Fuzzy("jokes",1));
    values.push_back(Fuzzy("chill",0.4));
    values.push_back(Fuzzy("unlock",0.9));
    conclusionPoolPC.AddConclusion(Conclusion("Need For Speed Underground 1 & 2",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.7));
    values.push_back(Fuzzy("oldschool",0.7));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0.2));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.2));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.3));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",1));
    values.push_back(Fuzzy("tuning",1));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.6));
    values.push_back(Fuzzy("jokes",1));
    values.push_back(Fuzzy("chill",0.4));
    values.push_back(Fuzzy("unlock",0.9));
    conclusionPoolPS.AddConclusion(Conclusion("Need For Speed Underground 1 & 2",values));
    values.clear();



    values.push_back(Fuzzy("challenging",1));
    values.push_back(Fuzzy("oldschool",0.5));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.5));
    values.push_back(Fuzzy("explore",0.7));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0.4));
    values.push_back(Fuzzy("portals",1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.7));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.4));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.9));
    values.push_back(Fuzzy("chill",0.6));
    values.push_back(Fuzzy("unlock",0.6));
    conclusionPoolPC.AddConclusion(Conclusion("Portal 1 & 2\nPortal Stories: Mel",values));
    values.clear();

    values.push_back(Fuzzy("challenging",1));
    values.push_back(Fuzzy("oldschool",0.5));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.5));
    values.push_back(Fuzzy("explore",0.7));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0.4));
    values.push_back(Fuzzy("portals",1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.7));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.4));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.9));
    values.push_back(Fuzzy("chill",0.6));
    values.push_back(Fuzzy("unlock",0.6));
    conclusionPoolPS.AddConclusion(Conclusion("Portal 1 & 2\nPortal Stories: Mel",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.5));
    values.push_back(Fuzzy("oldschool",0.9));
    values.push_back(Fuzzy("starwars",1));
    values.push_back(Fuzzy("story",0.8));
    values.push_back(Fuzzy("explore",0.4));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.5));
    values.push_back(Fuzzy("shooter",0.2));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.6));
    values.push_back(Fuzzy("adventure",0.2));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0.2));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.8));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.5));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.3));
    values.push_back(Fuzzy("unlock",0.6));
    conclusionPoolPC.AddConclusion(Conclusion("Star Wars: Jedi Academy",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.1));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",1));
    values.push_back(Fuzzy("story",0.8));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",1));
    values.push_back(Fuzzy("shooter",0.9));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0.9));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.9));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Star Wars Battlefront 2015",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0));
    values.push_back(Fuzzy("oldschool",0.9));
    values.push_back(Fuzzy("starwars",1));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",1));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",00));
    values.push_back(Fuzzy("shooter",0.2));
    values.push_back(Fuzzy("portals",1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",1));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.9));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0.2));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.7));
    values.push_back(Fuzzy("jokes",0.8));
    values.push_back(Fuzzy("chill",0.8));
    values.push_back(Fuzzy("unlock",0.9));
    conclusionPoolPS.AddConclusion(Conclusion("Lego Star Wars",values));
    values.clear();



    values.push_back(Fuzzy("challenging",0.2));
    values.push_back(Fuzzy("oldschool",0.9));
    values.push_back(Fuzzy("starwars",1));
    values.push_back(Fuzzy("story",0.8));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0.5));
    values.push_back(Fuzzy("shooter",1));
    values.push_back(Fuzzy("portals",1));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",0.6));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",1));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.8));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.1));
    values.push_back(Fuzzy("unlock",0));
    conclusionPoolPS.AddConclusion(Conclusion("Star Wars: Battlefront 2004",values));
    values.clear();



    values.push_back(Fuzzy("challenging",0.5));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0));
    values.push_back(Fuzzy("explore",0));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",1));
    values.push_back(Fuzzy("end",0));
    values.push_back(Fuzzy("adventure",0.1));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0.8));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",1));
    values.push_back(Fuzzy("jokes",0));
    values.push_back(Fuzzy("chill",0));
    values.push_back(Fuzzy("unlock",0.2));
    conclusionPoolPS.AddConclusion(Conclusion("Outlast",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.5));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.9));
    values.push_back(Fuzzy("explore",0.9));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.7));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.9));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Ori and the blind forest.\nBrothers: A Tale of Two sons.",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.6));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",1));
    values.push_back(Fuzzy("explore",1));
    values.push_back(Fuzzy("male",0.5));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0.8));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.9));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",1));
    values.push_back(Fuzzy("jokes",0.3));
    values.push_back(Fuzzy("chill",0.9));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("The Last of Us.",values));
    values.clear();

    values.push_back(Fuzzy("challenging",0.5));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.6));
    values.push_back(Fuzzy("explore",0.9));
    values.push_back(Fuzzy("male",0));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0.6));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.7));
    values.push_back(Fuzzy("lightsaber",0));
    values.push_back(Fuzzy("shooting",0.6));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0.1));
    values.push_back(Fuzzy("chill",0.4));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Tomb Raider.",values));
    values.clear();


    values.push_back(Fuzzy("challenging",0.3));
    values.push_back(Fuzzy("oldschool",0));
    values.push_back(Fuzzy("starwars",0));
    values.push_back(Fuzzy("story",0.9));
    values.push_back(Fuzzy("explore",0.9));
    values.push_back(Fuzzy("male",1));
    values.push_back(Fuzzy("czech",0));
    values.push_back(Fuzzy("2D",0));
    values.push_back(Fuzzy("PVP",0));
    values.push_back(Fuzzy("shooter",0));
    values.push_back(Fuzzy("portals",0));
    values.push_back(Fuzzy("gangster",0.3));
    values.push_back(Fuzzy("lego",0));
    values.push_back(Fuzzy("horror",0));
    values.push_back(Fuzzy("end",1));
    values.push_back(Fuzzy("adventure",0.7));
    values.push_back(Fuzzy("lightsaber",1));
    values.push_back(Fuzzy("shooting",0));
    values.push_back(Fuzzy("WW2",0));
    values.push_back(Fuzzy("racing",0));
    values.push_back(Fuzzy("tuning",0));
    values.push_back(Fuzzy("online",0));
    values.push_back(Fuzzy("MMORPG",0));
    values.push_back(Fuzzy("children",0));
    values.push_back(Fuzzy("cards",0));
    values.push_back(Fuzzy("graphics",0.9));
    values.push_back(Fuzzy("jokes",0.2));
    values.push_back(Fuzzy("chill",0.6));
    values.push_back(Fuzzy("unlock",0.8));
    conclusionPoolPS.AddConclusion(Conclusion("Uncharted.",values));
    values.clear();




    Game game = Game();
    game.AddQuestions(questionPoolPC, "PC");
    game.AddQuestions(questionPoolPS, "PS");
    game.AddConclusions(conclusionPoolPC, "PC");
    game.AddConclusions(conclusionPoolPS, "PS");
    game.StartGame();









/*
    Conclusions conlclusionPool = Conclusions();
    conlclusionPool.AddConclusion("0.");
    conlclusionPool.AddConclusion("1. Then go and play chess!!!!!");
    conlclusionPool.AddConclusion("2. Super Meat Boy");
    conlclusionPool.AddConclusion("3. Dark Souls, Nioh");
    conlclusionPool.AddConclusion("4. Mafia");
    conlclusionPool.AddConclusion("5. Vietcong");
    conlclusionPool.AddConclusion("6. Polda");
    conlclusionPool.AddConclusion("7. World of Warcraft");
    conlclusionPool.AddConclusion("8. League of Legends");
    conlclusionPool.AddConclusion("9. Hearthstone");
    conlclusionPool.AddConclusion("10. Counter Strike");
    conlclusionPool.AddConclusion("11. Crysis 1 & 2 & 3");
    conlclusionPool.AddConclusion("12. Undertale");
    conlclusionPool.AddConclusion("13. Max Payne");
    conlclusionPool.AddConclusion("14. Star Wars Battlefront (2015)");
    conlclusionPool.AddConclusion("15. Play a lego game then. Lego Star Wars is fun.");
    conlclusionPool.AddConclusion("16. Star Wars: Knights of the old republic");
    conlclusionPool.AddConclusion("17. Star Wars Battlefront (2004)");
    conlclusionPool.AddConclusion("18. F.E.A.R");
    conlclusionPool.AddConclusion("19. Outlast");
    conlclusionPool.AddConclusion("20. Call Of Duty WW2");
    conlclusionPool.AddConclusion("21. Ori and the blind forest.\nBrothers: A Tale of Two sons.");
    conlclusionPool.AddConclusion("22. Need for Speed Shift\nForza");
    conlclusionPool.AddConclusion("23. Need for Speed Underground 1 & 2");
    conlclusionPool.AddConclusion("24. The Last of Us");
    conlclusionPool.AddConclusion("25. Portal 1 & 2");
    conlclusionPool.AddConclusion("26. Tomb Raider");
    conlclusionPool.AddConclusion("27.Uncharted");



    //UPRAVIT KNOWLEDGE BASE.. josu spatne otazky


    vector <Relation> relationPool;
    relationPool.push_back(Relation( {1, 10, 11, 17}, 4));
    relationPool.push_back(Relation( {1, 10, 31}, 4));
    relationPool.push_back(Relation( {1, 10, 11}, 5));
    relationPool.push_back(Relation( {1, 10, 11, 21}, 6));
    relationPool.push_back(Relation( {1, 4, 5, 12}, 2));
    relationPool.push_back(Relation( {1, 4, 5}, 3));
    relationPool.push_back(Relation( {1, 10, 27, 28}, 7));
    relationPool.push_back(Relation( {1, 10, 27, 29}, 8));
    relationPool.push_back(Relation( {1, 10, 27, 30}, 9));
    relationPool.push_back(Relation( {1, 10, 27}, 10));
    relationPool.push_back(Relation( {1, 10, 32}, 11));
    relationPool.push_back(Relation( {1, 10, 33}, 12));
    relationPool.push_back(Relation( {1, 10}, 13));
    relationPool.push_back(Relation( {1, 4, 6, 13}, 14));
    relationPool.push_back(Relation( {1, 4, 6, 14}, 14));
    relationPool.push_back(Relation( {1, 4, 6, 18}, 15));
    relationPool.push_back(Relation( {1, 4, 6, 22}, 16));
    relationPool.push_back(Relation( {1, 4, 6}, 17));
    relationPool.push_back(Relation( {1, 4, 15, 19, 23}, 18));
    relationPool.push_back(Relation( {1, 4, 15, 19}, 19));
    relationPool.push_back(Relation( {1, 4, 15}, 20));
    relationPool.push_back(Relation( {1, 4, 34}, 21));
    relationPool.push_back(Relation( {1, 4, 25}, 22));
    relationPool.push_back(Relation( {1, 4, 25, 26}, 23));
    relationPool.push_back(Relation( {1, 4, 7, 20}, 24));
    relationPool.push_back(Relation( {1, 4, 7, 16}, 25));
    relationPool.push_back(Relation( {1, 4, 7, 8}, 26));
    relationPool.push_back(Relation( {1, 4, 7, 8, 9}, 27));




    Game game = Game();
    game.AddConclusions(conlclusionPool);
    game.AddQuestions(questionPool);
    game.AddRelations(relationPool);
    game.StartGame();





*/





    return 0;
}