#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/*  Complete all ** parts */

//==============================================
//  CS421 RE to NFA-e extra credit by: ** 
//===============================================



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
string NFAstring[10]; 

int state = 0;
int machineNum = 0;


// ------------- Put your utility functions here --------------------

void dispayTransition(transition t)
{
	cout << t.start << "--" << t.arrow << "--" << t.end << " " ;
}

void displayMachine(vector<transition> M)
{
	for (int i = 0; i < M.size(); i++)
	{
		dispayTransition(M[i]);
	}
	cout << "\nInitial= " << M.at(0).start << endl;
	cout << "Final= " << M.at(M.size() - 1).end ;
	cout << "\n=======================\n";
}

int copyMachine(vector<transition> M) {

	vector<transition> newM;
	transition newt;

	for (int i = 0; i < M.size(); i++)
	{
		newt.arrow = M[i].arrow;
		newt.start = state++;
		newt.end = state++;
		newM.push_back(newt);
	}
	
	NFA[machineNum] = newM;
	cout << "=======================\n";
	cout << "Machine" << machineNum << " :"  << endl;
	displayMachine(NFA[machineNum]);
	return machineNum++;
}

// ------------- Machine processing functions follow -----------

void processConcat()
{
	int M1, M2; // machine numbers 

	cout << "Enter number of the first machine:"; cin >> M1;
	cout << "Enter number of the second machine:"; cin >> M2;

	if (M1 == M2) {
		cout << "Concatenating with itself..." << endl;
		cout << "\n...copying the machine first ...." << endl;
		// Copy the machine with new state numbers and display it 
		// The copy should be treated as M2
		M2 = copyMachine(NFA[M1]);		
	}

	vector<transition> M; // the new machine to create and add to NFA
	transition t;

	//  For the new machine M to add to NFA:     
	   // - add M1 transitions and M2 transitions. 
	   // - add a transition from M1's end to M2's start.
	t.arrow = ' ';
	t.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t.end = NFA[M2].at(0).start;
	
	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i)); 
	}
	M.push_back(t);
	for (int i = 0; i < NFA[M2].size(); i++)
	{
		M.push_back(NFA[M2].at(i));
	}

	// Display the new concatenated machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for " ;

	stringstream sstm;
	sstm << "Machine" << M1 << "-Machine" << M2;
	string detail = sstm.str();
	cout << detail << endl ;
	NFAstring[machineNum] = detail;
	displayMachine(M);

	//  Add M to NFA.
	NFA[machineNum++] = M ;
	
} // end of concat


void processOr()
{
	int M1, M2; // machine numbers
	transition t1, t2, t3, t4;

	cout << "Enter number of the first machine:"; cin >> M1;
	cout << "Enter number of the second machine:"; cin >> M2;

	if (M1 == M2) { // must be a different one from M1
		cout << "Enter differnt number for the second machine";  cin >> M2;
	}

	vector<transition> M; // the new machine to create and add to NFA

	// ** For the new machine M to add to NFA:     
	   // - create a new start state. 
	   // - create a new final state.
	   // - add a transition from new start to M1 start.
	   // - add a transition from new start to M2 start.
	   // - add M1 transitions and M2 transitions. 
	   // - add a transition from M1's end to new final.
	   // - add a transition from M2's end to new final.

	t1.start = state;
	t2.start = state++;
	t3.end = state;
	t4.end = state++;

	t1.end = NFA[M1].at(0).start;
	t2.end = NFA[M2].at(0).start;

	t3.start = NFA[M1].at(NFA[M1].size() - 1).end;
	t4.start = NFA[M2].at(NFA[M2].size() - 1).end;

	t1.arrow = ' ';
	t2.arrow = ' ';
	t3.arrow = ' ';
	t4.arrow = ' ';

	M.push_back(t1);
	M.push_back(t2);

	for (int i = 0; i < NFA[M1].size(); i++)
	{
		M.push_back(NFA[M1].at(i));
	}
	for (int i = 0; i < NFA[M2].size(); i++)
	{
		M.push_back(NFA[M2].at(i));
	}
	M.push_back(t3);
	M.push_back(t4);


	// ** Display the machine M.
	cout << "=======================\n";
	cout << "Machine" << machineNum << " for ";

	stringstream sstm;
	sstm << "Machine" << M1 << "|Machine" << M2;
	string detail = sstm.str();
	cout << detail << endl;
	NFAstring[machineNum] = detail;
	displayMachine(M);

	//  Add M to NFA.
	NFA[machineNum++] = M;

}// end of OR 


void processStar()
{
	int M1;  // machine number

	cout << "Enter number of the machine:"; cin >> M1;

	vector<transition> M; // the new machine to create and add to NFA

	// ** For the new machine M to add to NFA:
	  // - create a new start state. 
	  // - create a new final state.
	  // - add a transition from the new start to M1's start.
	  // - add M1 transitions. 
	  // - add a transition (back) from M1's end to M1's start.
	  // - add a transition from M1's end to the new final.
	  // - add a transition (skip) from the new start to the new final.

	// ** Display the machine M.
	// ** Add the machine M to NFA.
} // end of STAR

void processPlus()
{
	int M1;  // machine number

	cout << "Enter number of the machine:"; cin >> M1;

	vector<transition> M; // the new machine to create and add to NFA

	// ** For the new machine M to add to NFA:
	  // - create a new start state. 
	  // - create a new final state.
	  // - add a transition from new start to M1's start.
	  // - add M1 transitions. 
	  // - add a transition (back) from M1's end to M1's start.
	  // - add a transition from M1's end to the new final.

	// ** Display the machine M.
	// ** Add the machine M to NFA.
} // end of PLUS


// ------------ Test Driver ---------------------

int main()
{
	cout << "This program will transform regular expressions into NFA-e" << endl;
	cout << "But it is done interactively by asking you to apply operators" << endl;
	cout << "to component machines." << endl;

	//ifstream fin("re.txt", ios::in);  // components come from re.txt
	ifstream fin("C:\\Users\\dever\\Desktop\\Spring2020\\TheoryComp\\cs421files\\CS421Progs\\DemosAndEC\\1_RE-NFAe\\re.txt", ios::in);  // components come from re.txt
	
	transition  t;
	int index = 0;
	char c;

	cout << "Reading in the components from re.txt...." << endl;
	cout << "=======================\n";
	string details;

	while (fin >> c)
	{
		transition trs;
		// Build a transition (trs) for arrow label c.
		trs.arrow = c;
		trs.start = state++;
		trs.end = state++;
		
		// Display the transition.
		vector<transition> M;
		M.push_back(trs);
		cout << "Machine" << machineNum << " for " << c << endl;
		details = c;
		
		displayMachine(M);

		// Add it to NFA as a machine.
		NFAstring[machineNum] = details;
		NFA[machineNum++] = M;
	}
	fin.close();
	// -- finished creating component machines

	// -- It then asks the user to combine machines.  
	// -- Combined machines' transitions will be added to NFA.

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

	cout << "Outputting all machines to nfa-e.txt... " << endl;

	//ofstream fout("nfa-e.txt", ios::out);
	ofstream fout("C:\\Users\\dever\\Desktop\\Spring2020\\TheoryComp\\EC\\nfa-e.txt", ios::out);
	
	//  Send all NFA contents to the output file which
	//    is created new each time.
	fout << "=======================\n";
	for (int j = 0; j < machineNum; j++) {
		fout << "Machine" << j << " for " << NFAstring[j]   << endl;
		for (int i = 0; i < NFA[j].size(); i++)
		{
			fout << NFA[j].at(i).start << "--" << NFA[j].at(i).arrow << "--" << NFA[j].at(i).end << " ";
		}
		fout << "\nInitial= " << NFA[j].at(0).start << endl;
		fout << "Final= " << NFA[j].at(NFA[j].size() - 1).end;
		fout << "\n=======================\n";
	}

} // the end
