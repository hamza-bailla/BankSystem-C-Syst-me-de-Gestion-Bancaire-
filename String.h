#pragma once
#include <iostream>
#include <vector>
using namespace std;


class String
{
	string _Value;

public:

	String() {}
	String(string Value)
	{
		_Value = Value;
	}


	void SetValue(string Value)
	{
		_Value = Value;
	}
	string GetValue()
	{
		return _Value;
	}
	__declspec(property(get = GetValue, put = SetValue)) string Value;


	//Count words Function
	static short CountWords(string Value)
	{
		short i;
		short counter = 0;
		string word;

		while ((i = Value.find(" ")) != std::string::npos)
		{
			word = Value.substr(0, i);
			counter++;
			Value.erase(0, i + 1);
		}

		if (Value != " ")
		{
			counter++;
		}

		return counter;
	}
	short CountWords()
	{
		return CountWords(_Value);
	}

	// Split Function
	static vector <string> split(string Value, string space)
	{
		vector <string> vspilt;
		string word;
		short i;

		while ((i = Value.find(space)) != std::string::npos)
		{
			word = Value.substr(0, i);
			vspilt.push_back(word);

			Value.erase(0, i + space.length());

		}

		if (Value != space)
		{
			vspilt.push_back(Value);
		}

		return vspilt;
	}
	vector <string> split(string space)
	{
		return split(_Value, space);
	}

	// Upper First Letter Function
	static string UpperFirstLetter(string Value)
	{
		bool isfirst = true;

		for (short i = 0; i < Value.length(); i++)
		{
			if (Value[i] != ' ' && isfirst == true)
			{
				Value[i] = toupper(Value[i]);
			}

			isfirst = (Value[i] == ' ') ? true : false;
		}
		return Value;
	}
	void UpperFirstLetter()
	{
		_Value = UpperFirstLetter(_Value);
	}

	// Lower First Letter Function
	static string LowerFirstLetter(string Value)
	{
		bool isfirst = true;

		for (short i = 0; i < Value.length(); i++)
		{
			if (Value[i] != ' ' && isfirst == true)
			{
				Value[i] = tolower(Value[i]);
			}

			isfirst = (Value[i] == ' ') ? true : false;
		}
		return Value;
	}
	void LowerFirstLetter()
	{
		_Value = LowerFirstLetter(_Value);
	}

	// Upper All String 
	static string UpperAllString(string Value)
	{
		for (short i = 0; i < Value.length(); i++)
		{
			Value[i] = toupper(Value[i]);
		}

		return Value;
	}
	void UpperAllString()
	{
		_Value = UpperAllString(_Value);
	}

	// Lower All String 
	static string LowerAllString(string Value)
	{
		for (short i = 0; i < Value.length(); i++)
		{
			Value[i] = tolower(Value[i]);
		}

		return Value;
	}
	string LowerAllString()
	{
		_Value = LowerAllString(_Value);
	}

	static string Encrypt(string pass, short key)
	{
		for (short i = 0; i <= pass.length(); i++)
		{
			pass[i] += key;
		}
		return  pass;
	}

	static string Decrypt(string encypt, short key)
	{

		for (short i = 0; i <= encypt.length(); i++)
		{
			encypt[i] -= key;
		}
		return  encypt;

	}
};

