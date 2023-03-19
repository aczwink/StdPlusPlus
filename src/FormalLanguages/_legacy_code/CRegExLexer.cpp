//Class Header
#include "__CRegExLexer.h"
//Namespaces
using namespace SJCLibIntern;

//Constructor
CRegExLexer::CRegExLexer() : CLexer<ERegExToken>(translate, (const uint32 *)transitions, tokenTypeMap)
{
}

//Private Functions
bool CRegExLexer::OnNextToken(const ERegExToken &refParsedToken)
{
	if(refParsedToken == REGEXTOKEN_INPUT && this->tokenValue.GetLength() != 1)
	{
		ASSERT(this->tokenValue[0] == '\\');
		if(this->tokenValue.GetLength() == 2)
		{
			switch(this->tokenValue[1])
			{
			case '\\':
			case '.':
			case '+':
			case '-':
			case '*':
			case '(':
			case ')':
			case '[':
			case ']':
			case '|':
				this->tokenValue = this->tokenValue[1];
				break;
			case 'f':
				this->tokenValue = '\f';
				break;
			case 'n':
				this->tokenValue = '\n';
				break;
			case 'r':
				this->tokenValue = '\r';
				break;
			case 't':
				this->tokenValue = '\t';
				break;
			default:
				ASSERT(0);
			}
		}
		else if(this->tokenValue.GetLength() == 4)
		{
			byte b;
			
			ASSERT(this->tokenValue[1] == 'x');
			
			if(this->tokenValue[2] >= 'A')
				b = (this->tokenValue[2] - 'A' + 10) << 4;
			else
				b = (this->tokenValue[2] - '0') << 4;
			
			if(this->tokenValue[3] >= 'A')
				b |= this->tokenValue[3] - 'A' + 10;
			else
				b |= this->tokenValue[3] - '0';
			
			this->tokenValue.Assign((char)b);
		}
		else
		{
			ASSERT(0);
		}
	}
	
	return true;
}