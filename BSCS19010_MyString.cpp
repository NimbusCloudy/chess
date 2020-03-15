#include <iostream>
#include <conio.h>
#include <fstream>
using namespace std;
#include "BSCS19010_MyStringClass.h"

int main()
{
	MyString Sample1("uama");
	MyString Sample2("qoq");
	MyString Sample3("99");
	cout << "\n Sample 1 :" << Sample1 << endl;
	cout << "\n Sample 2 :" << Sample2 << endl;
	cout << "\n Sample 3 :" << Sample3<<endl;
	int AllIndexCount = 0;
	//cout << "\nString concatenation\n";
	//cout << Sample1.str_cat(Sample2) << endl;
	/*cout << "\n\n String Compare :" << Sample1.CMP(Sample2) << endl << endl;
	cout << "\nSample1-------Tokenize------------\n";
	int tc = 0;
	const char* TS = "a";
	MyString* StringsTokenize = Sample1.Tokenize(TS, tc);
	for (int a = 0; a < tc; a++)
	{
		cout << "\n Tokenize String  " << a << ":  ";
		cout << " " << StringsTokenize[a] << " ";
	}
	cout << "\n-----------Split---------------------";
	int sc = 0;
	char split;
	cout << "\n Char:";
	cin >> split;
	MyString * SplitStrings =Sample2.Split(split, sc);
	for (int i = 0; i < sc; i++)
	{
		cout << "\n Split Strings " << i <<":   ";
		cout << SplitStrings[i];
	}
	cout << "\nSample1 Insert At String:";
	cout << "\n Insert At :";
	int ins;
	cin >> ins;
	Sample1.insert_at(ins, Sample2);
	cout << Sample1;
	cout << "\nSample2 Insert At(only char):";
	cout << "\n Insert At :";
	cin >> ins;
	char ch7;
	cout << "\n Insert Char :";
	cin >> ch7;
	Sample2.insert_at(ins, ch7);
	cout << Sample2;
	cout << "\nSample1 Trimmed:";
	Sample1.Trim();
	cout << Sample1;
	cout << "\nSample1 Remove At:";
	int rem;
	cout << "\nRemoved AT :";
	cin >> rem;
	Sample1.remove_at(rem);
	cout << Sample1 << endl;
	cout << "\nSample1 Find Character";
	cout << "\n Char :";
	 char ch1 ;
	cin >> ch1;
	cout << "\nSample1 Find First:";
	cout << Sample1.find_first(ch1)<< endl;
	cout << "\nSample1 Find Last:";
	cout << Sample1.find_last(ch1)<< endl;
	cout << "\nSample1 Find All:";
	int * A = Sample1.find_all(ch1, AllIndexCount);
	cout << "\n Count :" << AllIndexCount;
	cout << "\nAll Indexes :";
	for (int i=0 ; i < AllIndexCount;i++)
		cout << " " << A[i] << " ";
	cout << "\nCharacter To Remove\n";
	char chart = 'a';
	cout << "Char :";
	cin >> chart;
	cout << "\n Remove First:";
    Sample1.remove_first(chart);
	cout << Sample1;
	cout << "\n Remove Last:";
	cout << "Char :";
	char chart2;
	cin >> chart2;
	Sample2.remove_last(chart2);
	cout << Sample2;
	cout << "\n Remove All:";
	cout << "\nCharacter To Remove\n";
	cout << "Char :";
	char chart3;
	cin >> chart3;
	Sample2.remove_all(chart3);
	cout << "\n All Removed"<<Sample2;*/
	cout << "\n A to I (Sample 3) :";
	int val = MyString::Atoi(Sample3);
	cout << val+1;
	cout << "\n\n I to A ";
	int i = 123;
	cout << "\n Number:";
	cout << "Char to Int:";
	MyString A = MyString::Itoa(i);
	cout << A;
	return _getch();
}

//	cout << "\nSpaceCheck";
//	cout << "SpaceCheck\n";