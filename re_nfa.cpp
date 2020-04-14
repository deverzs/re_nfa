#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

//==========================================================
//  CS421 RE to NFA-e extra credit by: Zsuzsanna Dianovics
//==========================================================

/*
This program Takes re and converts it into NFA-e interactively

Algorithm:
   1. Read the re.txt file to create initial component machines
   2. Store these machines into NFA[]
   3. User entered operation is requested 
   4. Create the new machine based on input:
		- concatenate two machines
		+ - repeats re
		* repeats with option to return to first state from final
		| chooses one or the other machine path
   5. Created new machine added to NFA.
   6. Repeat as needed.
   7. NFA output to nfa-e.txt when user ends program

*/

// Each transition within a machine
struct transition
{
	int start;    // start state
	char arrow;   // what to see on an arrow
	int end;      // next state
};

// NFA can store up to 10 machines.  
// Each machine is a vector of transitions.
vector<transition> NFA[10];

//holds the machines creation status
string NFAstring[10]; 

//states of the transitions
int state = 0;

//number of machines
int machineNum = 0;


// ------------- Put your utility functions here --------------------

//Output a transition
// @param - t transition to print
void dispayTransition(transition t)
{
	cout << t.start << "--" << t.arrow << "--" << t.end << " " ;
}

//displays the machine
// @param M the vector of transitions
// @calls displayTransition
void displayMachine(vector<transition> M)
{
	//loop through machine
	for (int i = 0; i < M.size(); i++)
	{
		dispayTransition(M[i]);
	}

	//output final end initial states
	cout << "\nInitial= " << M.at(0).start << endl;
	cout << "Final= " << M.at(M.size() - 1).end ;
	cout << "\n=======================\n";
}


//copies the input machine
// @param M the vector of transitions
// @returns int that is the new machinine number
int copyMachine(vector<transition> M) {

	//new machine
	vector<transition> newM;

	//new transition
	transition newt;

	//loop through the machine 
	//create new transitions and the transitions to the new machine
	for (int i = 0; i < M.size(); i++)
	{
		newt.arrow = M[i].arrow;
		newt.start = state++;
		newt.end = state++;
		newM.push_back(newt);
	}
	
	//add new machine to NFA
	NFA[machineNum] = newM;
	cout << "=======================\n";
	cout << "Machine" << machineNum << " :"  << endl;

	//display the machine
	displayMachine(NFA[machineNum]);

	//return the number of the machine and increment
	return machineNum++;
}

// ------------- Machine processing functions follow -----------

//Concatenation of two machines
//	get user input
//	copy a machine if the machine numbers are the same
//  Create new machine M to add to NFA:     
// - add M1 transitions and M2 transitions. 
// - add a transition from M1's end to M2's start.
void processConcat()
{
	int M1, M2; // machine numbers 

	cout << "Enter number of the first machine:"; cin >> M1;
	cout << "Enter number of the second machine:"; cin >> M2;

	//copy the machine
	if (M1 == M2) {
		cout << "Concatenating with itself..." << endl;
		cout << "\n...copying the machine first ...." << endl;

		// Copy the machine with new state numbers and display it 
		M2 = copyMachine(NFA[M1]);		
	}

	// the new machine to create and add to NFA
	vector<transition> M; 

	//new transition
	transition t;

	t.arrow = ' ';

	//connect the machines
	t.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t.end = NFA[M2].at(0).start;
	
	//add to the new machine M1
	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i)); 
	}
	//to the new machine the epsiol transition
	M.push_back(t);
	//add to the new machine M2
	for (int i = 0; i < NFA[M2].size(); i++)
	{
		M.push_back(NFA[M2].at(i));
	}

	// Display the new concatenated machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for " ;

	//create a string to decscibe the machine and add to NFAstring[]
	stringstream sstm;
	sstm << "Machine" << M1 << "-Machine" << M2 << " :";
	string detail = sstm.str();
	cout << detail << endl ;
	NFAstring[machineNum] = detail;

	displayMachine(M);

	//  Add M to NFA.
	NFA[machineNum++] = M ;
	
} // end of concat


//Or of two machines
//	get user input
//  Create new machine M to add to NFA:     
// - create a new start state. 
// - create a new final state.
// - add a transition from new start to M1 start.
// - add a transition from new start to M2 start.
// - add M1 transitions and M2 transitions. 
// - add a transition from M1's end to new final.
// - add a transition from M2's end to new final.
void processOr()
{
	// machine numbers
	int M1, M2; 
	//new transtitions
	transition t1, t2, t3, t4;

	cout << "Enter number of the first machine:"; cin >> M1;
	cout << "Enter number of the second machine:"; cin >> M2;

	//Machines entered must be a different one from M1
	if (M1 == M2) { 
		cout << "Enter differnt number for the second machine";  cin >> M2;
	}

	// the new machine to create and add to NFA
	vector<transition> M; 

	//create new transtions to add to new machine
	t1.start = state;
	t2.start = state++;
	t3.end = state;
	t4.end = state++;

	t1.end = NFA[M1].at(0).start;
	t2.end = NFA[M2].at(0).start;

	t3.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t4.start = NFA[M2].at(NFA[M2].size() - 1).end;

	//adding epsion moves
	t1.arrow = ' ';
	t2.arrow = ' ';
	t3.arrow = ' ';
	t4.arrow = ' ';

	//first add t1 and t2 to the new machine
	M.push_back(t1);
	M.push_back(t2);

	//add the machines M1 and M2
	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i));
	}
	for (int i = 0; i < NFA[M2].size(); i++)
	{
		M.push_back(NFA[M2].at(i));
	}

	//add the last two transitions of epsilon
	M.push_back(t3);
	M.push_back(t4);

	//  Display the machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for ";

	//create a string to decscibe the machine and add to NFAstring[]
	stringstream sstm;
	sstm << "Machine" << M1 << "|Machine" << M2 << " :";
	string detail = sstm.str();
	cout << detail << endl;
	NFAstring[machineNum] = detail;

	displayMachine(M);

	// Add M to NFA.
	NFA[machineNum++] = M;

}// end of OR 

//Star of two machines
//	get user input
//  For the new machine M to add to NFA:
// - create a new start state. 
// - create a new final state.
// - add a transition from the new start to M1's start.
// - add M1 transitions. 
// - add a transition (back) from M1's end to M1's start.
// - add a transition from M1's end to the new final.
// - add a transition (skip) from the new start to the new final.
void processStar()
{
	// machine number
	int M1; 

	cout << "Enter number of the machine:"; cin >> M1;

	// the new machine to create and add to NFA
	vector<transition> M; 

	//new transitions to add to the new machine
	transition t1, t2, t3, t4;

	t1.start = state;
	t2.start = state++;

	t3.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t4.start = NFA[M1].at(NFA[M1].size() - 1).end;

	t2.end = state;
	t3.end = state++;
		
	t4.end = NFA[M1].at(0).start;
	t1.end = NFA[M1].at(0).start;

	//adding epsion moves
	t1.arrow = ' ';
	t2.arrow = ' ';
	t3.arrow = ' ';
	t4.arrow = ' ';

	//add the first transition to the machine
	M.push_back(t1);

	// add M1
	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i));
	}

	//add the rest of the transitions
	M.push_back(t4);
	M.push_back(t3);
	M.push_back(t2);
	
	//  Display the machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for ";

	//create a string to decscibe the machine and add to NFAstring[]
	stringstream sstm;
	sstm << "Machine" << M1 << " * :";
	string detail = sstm.str();
	cout << detail << endl;
	NFAstring[machineNum] = detail;

	displayMachine(M);

	//  Add M to NFA.
	NFA[machineNum++] = M;

} // end of STAR


//Plus of two machines
//	get user input
//  For the new machine M to add to NFA:
// - create a new start state. 
// - create a new final state.
// - add a transition from new start to M1's start.
// - add M1 transitions. 
// - add a transition (back) from M1's end to M1's start.
// - add a transition from M1's end to the new final.
void processPlus()
{
	// machine number
	int M1;  

	cout << "Enter number of the machine:"; cin >> M1;

	// the new machine to create and add to NFA
	vector<transition> M;

	//transitions to add to new machine
	transition t1, t2, t3, t4;

	t1.start = state++;

	t3.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t4.start = NFA[M1].at(NFA[M1].size() - 1).end;

	t3.end = state++;

	t4.end = NFA[M1].at(0).start;
	t1.end = NFA[M1].at(0).start;

	//adding epsilon moves
	t1.arrow = ' ';
	t3.arrow = ' ';
	t4.arrow = ' ';

	//add the first transitin
	M.push_back(t1);

	// add M1
	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i));
	}

	//add rest of transitions
	M.push_back(t4);
	M.push_back(t3);

	//  Display the machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for ";

	//create a string to decscibe the machine and add to NFAstring[]
	stringstream sstm;
	sstm << "Machine" << M1 << " + :";
	string detail = sstm.str();
	cout << detail << endl;
	NFAstring[machineNum] = detail;
	displayMachine(M);

	//  Add M to NFA.
	NFA[machineNum++] = M;

} // end of PLUS


// ------------ Test Driver ---------------------

int main()
{
	cout << "This program will transform regular expressions into NFA-e" << endl;
	cout << "But it is done interactively by asking you to apply operators" << endl;
	cout << "to component machines." << endl;

	ifstream fin("re.txt", ios::in);  // components come from re.txt
	//ifstream fin("C:\\Users\\dever\\Desktop\\Spring2020\\TheoryComp\\cs421files\\CS421Progs\\DemosAndEC\\1_RE-NFAe\\re.txt", ios::in);  // components come from re.txt
	
	transition  t;	//hold a transition
	char c;		//hold the arrow/move

	cout << "Reading in the components from re.txt...." << endl;
	cout << "=======================\n";
	string details;

	//read the file
	while (fin >> c)
	{
		// Build a transition (trs) for arrow label c.
		//increment states as move forward 
		transition trs;		
		trs.arrow = c;
		trs.start = state++;
		trs.end = state++;
		
		// new machine to add transition to
		vector<transition> M;
		M.push_back(trs);
		cout << "Machine" << machineNum << " for " << c << endl;
		details = c;
		
		//display the new machine
		displayMachine(M);

		//add the string of moves to NFAstring
		NFAstring[machineNum] = details;

		// Add it to NFA as a machine.
		NFA[machineNum++] = M;
	}
	fin.close();
	// finished creating component machines
 
	// Combined machines' transitions will be added to NFA.
	//Print menu for user
	char A = 'a';  // user choice
	while (A != 'n')
	{
		cout << "---- MENU -----" << endl;
		cout << "* for a machine" << endl;
		cout << "+ for a machine" << endl;
		cout << "| for two machines" << endl;
		cout << "- for appending two machines" << endl;
		cout << "n to quit" << endl;
		cin >> A;

		if (A == '*') processStar();
		else if (A == '+') processPlus();
		else if (A == '|') processOr();
		else if (A == '-') processConcat();

	}// end of interaction

	//once user exits, print NFA machines to file
	cout << "Outputting all machines to nfa-e.txt... " << endl;
	
	ofstream fout("nfa-e.txt", ios::out);
	//ofstream fout("C:\\Users\\dever\\Desktop\\Spring2020\\TheoryComp\\EC\\nfa-e.txt", ios::out);
	
	//  Send all NFA contents to the output file which
	//    is created new each time.
	fout << "=======================\n";

	//loop throuth NFA[]
	for (int j = 0; j < machineNum; j++) {
		fout << "Machine" << j << " for " << NFAstring[j]   << endl;

		//loop through each machine
		for (int i = 0; i < NFA[j].size(); i++)
		{
			fout << NFA[j].at(i).start << "--" << NFA[j].at(i).arrow << "--" << NFA[j].at(i).end << " ";
		}
		fout << "\nInitial= " << NFA[j].at(0).start << endl;
		fout << "Final= " << NFA[j].at(NFA[j].size() - 1).end;
		fout << "\n=======================\n";
	}

} // the end
