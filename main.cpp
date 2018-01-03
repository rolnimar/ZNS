#include <iostream>
#include <string>
#include <vector>

using namespace std;


//-------------------------------------QUESTIONS---------------------------------------------------
class Question{
public:
    Question(const string&, const double&);
    ~Question(){};
    string getQuestion();
    double getWeight();

private:
    string question;
    double weight;

};

Question::Question(const string & question, const double & weight) {
    this->question = question;
    this->weight = weight;
}

string Question::getQuestion() {
    return question;
}

double Question::getWeight() {
    return weight;
}


class Questions{
public:
    Questions();
    ~Questions();
    void AddQuestion(const Question&);
    string getQuestion(const int&);
    double getWeight(const int&);


private:
    vector <Question> questions;
};

Questions::Questions() {
}

Questions::~Questions() {
}

void Questions::AddQuestion(const Question& question) {
    questions.push_back(question);
}

string Questions::getQuestion(const int & index) {
    return questions[index].getQuestion();
}

double Questions::getWeight(const int& index) {
    return questions[index].getWeight();
}



//-------------------------------------CONCLUSIONS---------------------------------------------------
class Conclusions{
public:
    Conclusions(){};
    ~Conclusions(){};
    void AddConclusion(const string&);
    string getConclusion(const int&);
private:
    vector <string> conclusions;
};

void Conclusions::AddConclusion(const string& conclusion)  {
    conclusions.push_back(conclusion);

}

string Conclusions::getConclusion(const int & index) {
    return conclusions[index];
}

//-------------------------------------RELATIONS---------------------------------------------------
class Relation{
public:
    Relation(vector <int>, const int);
    vector <int> getPath();
    int getConclusion();
    void eraseFirst();
    int getSize();
private:
    vector <int> path;
    int conclusion;

};


void Relation::eraseFirst() {
    path.erase(path.begin());
}
vector <int> Relation::getPath() {
    return path;
}

Relation::Relation(vector <int>  x, const int  y) {
    path = x;
    conclusion = y;
    //cout << conclusion << "CONLCUSIONS"<<endl;
}

int Relation::getSize() {
    return path.size();
}

int Relation::getConclusion() {
    return conclusion;
}
//-------------------------------------GAME---------------------------------------------------

class Game{
public:
    Game(){};
    ~Game(){};
    void StartGame();
    void AddQuestions( Questions);
    void AddConclusions (Conclusions);
    void AddRelations (vector <Relation>);


private:
    Questions questionPool;
    Conclusions conclusionPool;
    vector<Relation> relationPool;
    void getInput();
    string input;
    int questionNum=0;

};


void Game::getInput() {
    cin >> input;
}


//inference system doesnt work, gotta make a new solution.
//need to ask the teacher if the problem can be solved as a graph problem..
void Game::StartGame() {



    int CurrentQuestion = 1;
    cout << questionPool.getQuestion(CurrentQuestion) << "  " << questionPool.getWeight(CurrentQuestion)<<endl;

    //cout << relationPool[0].getPath()[0] << endl;


    while (1){

/*
        for(auto it = relationPool.begin();it!=relationPool.end();it++){
            for(auto it2 = (*it).getPath().begin();it2!=(*it).getPath().end();it2++){

                cout << *it2 << " ";
            }
            cout << endl;
        }
*/
        getInput();
        if(input!="Yes" && input != "No" && input != "yes" && input!="no")
            cout << "Anwer either yes or no"<< endl;
        else{
            if(input == "no" || input == "No") {

                for(auto it = relationPool.end()-1;it!=relationPool.begin()-1;it--){
                    if(it->getPath().size()==0){

                        continue;
                    } else {

                        if (it->getPath()[0] == CurrentQuestion) {
                            //cout << it->getConclusion() << " ALSDJAKSJDHAKSJ " << endl;
                            relationPool.erase(it);
                        } else {


                           continue;

                        }
                    }
                }

                if(relationPool.size()==1){
                    cout << "Your game is:"<< endl << conclusionPool.getConclusion(relationPool[0].getConclusion()) << endl;
                    break;
                }
                if(relationPool.size()==0){
                    cout << conclusionPool.getConclusion(1) << endl;
                    break;
                }
                int i = 0;
                while(1) {
                    //cout << " PADA " << endl;
                    if(!relationPool[i].getPath().empty()) {

                        CurrentQuestion = relationPool[i].getPath()[0];
                        break;

                    }
                    i++;

                }
                cout << questionPool.getQuestion(CurrentQuestion) << "  " << questionPool.getWeight(CurrentQuestion)<<endl;
            } else {

                for(auto it = relationPool.end()-1;it!=relationPool.begin()-1;it--){

                    if(it->getPath().size()==0){
                        relationPool.erase(it);
                    }else {
                        if (it->getPath()[0] != CurrentQuestion) {
                           // cout << " this is ELEMENT0 " << it->getPath()[0] << endl ;
                            //cout <<   "                  GAME IS " << conclusionPool.getConclusion((*it).getConclusion()) << endl;
                            relationPool.erase(it);
                        } else {
                            it->eraseFirst();
                        }
                    }


                }


                if(relationPool.size()==1){
                    cout << "Your game is:"<< endl << conclusionPool.getConclusion(relationPool[0].getConclusion())<<endl;
                    break;
                }

                int i = 0;
                while(1) {
                    //cout << " PADA " << endl;
                    if(!relationPool[i].getPath().empty()) {

                        CurrentQuestion = relationPool[i].getPath()[0];
                        break;

                    }
                    i++;

                }
                //cout << "CURENT QUESTION SET IS " <<CurrentQuestion<< endl;
                cout << questionPool.getQuestion(CurrentQuestion) << "  " << questionPool.getWeight(CurrentQuestion)<<endl;
            }
        }
        //cout << "relations" << endl;
/*
        for(auto it = relationPool.begin();it!= relationPool.end();it++){
            cout << endl;/*
            for(auto it2 = it->getPath().begin();it2!=it->getPath().end();it2++){
                cout << *it2 << " ";
            }
            //cout << it->getPath()[0] << "    ";

            cout << "CONCL " <<it->getConclusion() << " SIZE " <<it->getSize();


            cout <<   "                  GAME IS " << conclusionPool.getConclusion((*it).getConclusion()) << "SIZE " <<it->getSize();
        }

        cout << endl<<"current question " << CurrentQuestion << endl;
*/

    }
}


void Game::AddRelations (vector <Relation> relations){
    this->relationPool = relations;
}

void Game::AddConclusions(Conclusions conclusions) {
    this->conclusionPool = conclusions;
}

void Game::AddQuestions( Questions  questions) {
    this->questionPool = questions;
}

//-------------------------------------MAIN---------------------------------------------------

int main(){

    Questions questionPool = Questions();
    questionPool.AddQuestion(Question("0.",1));
    questionPool.AddQuestion(Question("1. Do you want to play a game?",0.95));
    questionPool.AddQuestion(Question("2. DummyQ",1));
    questionPool.AddQuestion(Question("3. DummyQ",1));
    questionPool.AddQuestion(Question("4. Do you want to play a console game?",0.35));
    questionPool.AddQuestion(Question("5. Do you like challenging games?",0.75));
    questionPool.AddQuestion(Question("6. What about Star Wars. Do you like SW?",0.7));
    questionPool.AddQuestion(Question("7. Shame on you. Do you want to experience a good story in a game?",0.55));
    questionPool.AddQuestion(Question("8. Do you want to explore and jump a lot?",0.35));
    questionPool.AddQuestion(Question("9. Do you want to play a male character?",0.5));
    questionPool.AddQuestion(Question("10. Do you want to play a PC game?",0.65));
    questionPool.AddQuestion(Question("11. Do you want to play a czech game?",0.7));
    questionPool.AddQuestion(Question("12. Do you want to play a 2D game?",0.2));
    questionPool.AddQuestion(Question("13. Do you want to play PvP?",0.4));
    questionPool.AddQuestion(Question("14. PvP is fun though, want to change your mind?",0.2));
    questionPool.AddQuestion(Question("15. Do you like shooters?",0.6));
    questionPool.AddQuestion(Question("16. Do you want to go through portals?",0.6));
    questionPool.AddQuestion(Question("17. Do you want to be a gangster?",0.8));
    questionPool.AddQuestion(Question("18. Do you want to play a lego game?",0.2));
    questionPool.AddQuestion(Question("19. Do you like horror games?",0.1));
    questionPool.AddQuestion(Question("20. Do you never want the game to end when you finish playing it?",0.9));
    questionPool.AddQuestion(Question("21. Do you want to play adventure game?",0.2));
    questionPool.AddQuestion(Question("22. Do you want to use a lightsaber?",0.7));
    questionPool.AddQuestion(Question("23. Do you want shooting to be in the game?",0.6));
    questionPool.AddQuestion(Question("24. Do you want to be a part of  WW2?",0.3));
    questionPool.AddQuestion(Question("25. Do you like racing games?",0.4));
    questionPool.AddQuestion(Question("26. Do you want to tune your car?",0.8));
    questionPool.AddQuestion(Question("27. Do you want to play online?",0.7));
    questionPool.AddQuestion(Question("28. Do you like MMORPGs?",0.4));
    questionPool.AddQuestion(Question("29. Do you like playing against a bunch of children?",0.1));
    questionPool.AddQuestion(Question("30. Do you like cards?",0.3));
    questionPool.AddQuestion(Question("31. Do you want to play a good oldschool game?",0.4));
    questionPool.AddQuestion(Question("32. Do you want to play a game with good graphics?",0.8));
    questionPool.AddQuestion(Question("33. Do you want to play a game with a lot of jokes?",0.8));
    questionPool.AddQuestion(Question("34. Do you want to play a super chill game and want to have fun?",0.9) );


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











    return 0;
}