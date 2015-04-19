// reading.cpp
// Methods for input reading and processing


using namespace std;

#ifndef types_cpp
	#include "types.cpp"
#endif
#define reading_cpp


// Returns string up to comment (delimited by '#')
string getLineNoCommentLowercase(string in)
{
	// try using strtok to avoid converting to stream!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	string tempStr;
	stringstream data;
	data << in;
	getline(data, tempStr, '#');
	transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
	return tempStr;
}

ElementType getType(string in)
{
	if(in.back() == ':') // Would change order of directive/label increase efficiency?
		return Label;
	if(in.front() == '.')
		return Directive;
	return Instruction; // Or content!!!!!! Maybe say it's invalid type, depending on implementation
}

string splitLabel(string *in)
{
	int pos = in->find(':');
	if(pos == -1) return "";
	*in = in->substr(pos + 1);
	return in->substr(0, pos + 1);
}


void tokenize_old(char *in, TokenContainer *TC)
{
	int i = 0;
	stringstream data;
	char *token;

	token = strtok(in, ", \t");

	while(token != NULL)
	{
		(TC->tokens[i++]).assign(token);
		//cout << token << '|';
		token = strtok(NULL, ", \t");
	}
	TC->amount = i; // Store number of tokens on zero index
	//cout << TC->amount << '|';
	//for(i=0; i < 5; i++)
	//{
	//	cout << TC->tokens[i] << '|';
	//}
	//cout << endl;
}

TokenContainer tokenize(string in)
{
	TokenContainer TC;
	TC.amount = 0;
	if(in == "") return TC;

	int i = 0;
	char tempCA[50];
	strcpy(tempCA, in.c_str());
	char *token;
	

	token = strtok(tempCA, ", \t");

	while(token != NULL)
	{
		(TC.tokens[i++]).assign(token);
		//cout << token << '|';
		token = strtok(NULL, ", \t");
	}
	TC.amount = i; // Store number of tokens on zero index
	//cout << TC->amount << '|';
	//for(i=0; i < 5; i++)
	//{
	//	cout << TC->tokens[i] << '|';
	//}
	//cout << endl;
	return TC;
}


Element* getElement(TokenContainer TC)
{
	if(TC.amount == 0) return NULL;
	if(getType(TC.tokens[0]) == Label)
	{
		Element *_element = new Element(TC.tokens[0]);
		cout << "Label: " << _element->content << endl;
		return _element;
	}
	if(getType(TC.tokens[0]) == Directive)
	{
		DirectiveType _dir;
		if(TC.tokens[0] == ".org") _dir = Org;
		else if(TC.tokens[0] == ".align") _dir = Align;
		else if(TC.tokens[0] == ".wfill") _dir = Wfill;
		else if(TC.tokens[0] == ".word") _dir = Word;
		else if(TC.tokens[0] == ".set") _dir = Set;
		else return NULL;
		Element *_element = new Element(_dir, (TC.tokens[1] + ',' + TC.tokens[2]));
		cout << "Directive: " << _element->dir << " Content: " << _element->content << endl;
		return _element;
	}
	OpCodeType _op;
	if (TC.tokens[0] == "ldmq") _op = Load_MQ;
	else if (TC.tokens[0] == "ldmqm") _op = Load_MQ_MX;
	else if (TC.tokens[0] == "str") _op = Stor_MX;
	else if (TC.tokens[0] == "load") _op = Load_MX;
	else if (TC.tokens[0] == "ldn") _op = Load_MX_neg;
	else if (TC.tokens[0] == "ldabs") _op = Load_MX_mod;
	else if (TC.tokens[0] == "jmp") _op = Jump_M_L; // Do not need L/R versions
	else if (TC.tokens[0] == "jmp") _op = Jump_M_R; // Do not need L/R versions
	else if (TC.tokens[0] == "jgez") _op = Jump_M_cond_L; // Do not need L/R versions
	else if (TC.tokens[0] == "jgez") _op = Jump_M_cond_R; // Do not need L/R versions
	else if (TC.tokens[0] == "add") _op = Add_MX;
	else if (TC.tokens[0] == "addabs") _op = Add_MX_mod;
	else if (TC.tokens[0] == "sub") _op = Sub_MX;
	else if (TC.tokens[0] == "subabs") _op = Sub_MX_mod;
	else if (TC.tokens[0] == "mul") _op = Mul_MX;
	else if (TC.tokens[0] == "div") _op = Div_MX;
	else if (TC.tokens[0] == "lsh") _op = Lsh;
	else if (TC.tokens[0] == "rsh") _op = Rsh;
	else if (TC.tokens[0] == "stm") _op = Stor_M_L; // Do not need L/R versions
	else if (TC.tokens[0] == "stm") _op = Stor_M_R; // Do not need L/R versions
	else return NULL;
	Element *_element = new Element(_op, TC.tokens[1]);
	cout << "Instruction: " << _element->opcode << " Content: " << _element->content << endl;

	return NULL;
}


//string *tokenize(char *in)
//{
//	string tokens[5];
//	int i = 0;
//	stringstream data;
//	char *label, *directive, *instruction, *aux;
//
//	label = strtok(in, ":");
//	remaining = strtok(NULL, ".");
//	if(remaining == NULL) //INVALID!! Label is only element of line
//	{
//
//
//	}
//	dir_or_int = strtok(remaining, ".");
//	remaining = strtok(NULL, ".");
//	if(remaining == NULL) // Instruction is only remaining element of line
//	{
//		// Parse instruction:
//
//	}
//	// Parse directive:
//	cout << (token == NULL) << endl;
//
//	while(token != NULL)
//	{
//		data << token << '|';
//
//		//if(getType(token) == label)
//		//{
////
//		//}
//		//else if
//
//		token = strtok(NULL, "5");
//	}
//	cout << data.rdbuf() << endl;
//}

//string *tokenize_old(char *in)
//{
//	string tokens[5];
//	int i = 0;
//	stringstream data;
//	char *token;
//
//	//cout << "|" << strtok(in, "(") << "|" << endl;
//
//	token = strtok(in, ", \t");
//
//	while(token != NULL)
//	{
//		cout << token << '|';
//		token = strtok(NULL, ", \t");
//	}
//	cout << token << endl;
//}