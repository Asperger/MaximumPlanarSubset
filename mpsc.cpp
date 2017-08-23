#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int,int> range;

int num_point;	
map<int,int> table_chord;                //Store every chords given, and the index is one end of the chord
map<range,int> table_result;             //Store maximum number with different range
vector<int> result_chord;                //Store those chords found in MIS

//Edit the number of each point
void Rotate(int degree, int num, map<int,int>& table){
	map<int,int> new_table;
	for (map<int,int>::iterator it = table.begin(); it != table.end(); it++)
		new_table[(it->first + degree) % num] = (it->second + degree) % num;
	table = new_table;
}

//Edit the number of each point
void Rotate(int degree, int num, vector<int>& storage){
	for (int i = 0; i < storage.size(); i++)
		storage[i] = (storage[i] + degree) % num;
}

//Perform the algorithm
int MIS(const int i, const int j){
	if (j <= i) return 0; //trivial
	else if (table_result.find(range(i,j)) == table_result.end()){
		map<int,int>::const_iterator it = table_chord.find(i);
		map<int,int>::const_iterator jt = table_chord.find(j);
		if (it == table_chord.end() && jt == table_chord.end()){
			//case nothing
			table_result[range(i,j)] = MIS(i+1, j-1);
			return table_result[range(i,j)];
		}
		else if (it->second == jt->first && it->first == jt->second){
			//case 3
			result_chord.push_back(it->first); //one definitely not across other chords
			table_result[range(i,j)] = MIS(i+1, j-1) + 1;
			return table_result[range(i,j)];
		}
		else if (jt != table_chord.end()){
			//similar as below
			int k = jt->second;
			if (k > jt->first || k < it->first){
				//case 1
				table_result[range(i,j)] = MIS(i, j-1);
				return table_result[range(i,j)];
			}
			else {
				//case 2
				table_result[range(i,j)] = MIS(i, k-1) + MIS(k, j);
				return table_result[range(i,j)];
			}
		}
		else if (it != table_chord.end()){
			//similar as above
			int k = it->second;
			if (k > jt->first || k < it->first){
				//case 1
				table_result[range(i,j)] = MIS(i+1, j);
				return table_result[range(i,j)];
			}
			else {
				//case 2
				table_result[range(i,j)] = MIS(i, k) + MIS(k+1, j);
				return table_result[range(i,j)];
			}
		}
	}
	else return table_result[range(i,j)]; //computed
}

int main(int argc, char* argv[]){
	int i, j, max_num = 0;
	vector<int> max_result_chord;
	fstream fin;
	fstream fout;
	
	//Check format of command line
	if (argc != 3){
		cout << "Illegal command" << endl;
		return 0;
	}
	fin.open(argv[1],fstream::in);
	if (!fin.is_open()){
		cout << "Invalid input file name" << endl;
		return 0;
	}
	fout.open(argv[2],fstream::out);
	if (!fout.is_open()){
		cout << "Invalid output file name" << endl;
		return 0;
	}
	
	//Read input file
	fin >> num_point;
	while (fin >> i >> j){
		table_chord[i] = j;
		table_chord[j] = i;
	}
	fin.close();
	
	//Compute result with different rotation
	for (i = 0; i < num_point; i++){
		table_result.clear();
		result_chord.clear();
		int num = MIS(0, num_point-1);
		Rotate(1, num_point, table_chord);
		if (num > max_num){
			Rotate((num_point - i), num_point, result_chord);
			max_result_chord = result_chord;
			max_num = num;
		}
	}
	
	//Write the result
	fout << max_num << endl;
	sort(max_result_chord.begin(), max_result_chord.end());
	for (i = 0; i < max_result_chord.size(); i++)
		fout << max_result_chord[i] << " " << table_chord[max_result_chord[i]] << endl;
	fout.close();
	return 0;
} 
