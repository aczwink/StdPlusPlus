/*
T should be any primitive type
*/
/*
template<typename T>
class CHuffmanCodeNode
{
public:
	//Members
	bool isLeaf;
	uint32 frequency;
	T symbol;
	CHuffmanCodeNode<T> *pLeft;
	CHuffmanCodeNode<T> *pRight;
	CHuffmanCodeNode<T> *pParent;
	//Constructors
	CHuffmanCodeNode(uint32 frequency, T symbol)
	{
		this->isLeaf = true;
		this->frequency = frequency;
		this->symbol = symbol;
		this->pLeft = NULL;
		this->pRight = NULL;
		this->pParent = NULL;
	}

	CHuffmanCodeNode(uint32 frequency, CHuffmanCodeNode<T> *pLeft, CHuffmanCodeNode<T> *pRight)
	{
		this->isLeaf = false;
		this->frequency = frequency;
		this->pLeft = pLeft;
		this->pRight = pRight;
		this->pParent = NULL;

		pLeft->pParent = this;
		pRight->pParent = this;
	}
	
	//Inline
	inline uint32 GetFrequency() const
	{
		return this->frequency;
	}

	inline T GetSymbol() const
	{
		return this->symbol;
	}

	inline bool IsLeaf() const
	{
		return this->isLeaf;
	}
};

template<typename T>
class CHuffmanCoder
{
private:
	//Members
	CHuffmanCodeNode<T> *pRoot;
	CPriorityQueue<CHuffmanCodeNode<T> *> leafesQueue;
	CPriorityQueue<CHuffmanCodeNode<T> *> nodesQueue;
	CMap<T, CHuffmanCodeNode<T> *> symbolsMap;
public:
	//Methods
	void Build()
	{
		uint32 frequency;
		CHuffmanCodeNode<T> *p1, *p2;
		
		while(nodesQueue.GetNoOfElements() > 1)
		{
			p1 = nodesQueue.PopFirst();
			p2 = nodesQueue.PopFirst();
			frequency = p1->GetFrequency() + p2->GetFrequency();
			
			nodesQueue.Insert(frequency, new CHuffmanCodeNode<T>(frequency, p1, p2));
		}
		
		this->pRoot = nodesQueue.PopFirst();
	}

	T Decode(uint32 code)
	{
		CHuffmanCodeNode<T> *pNode;

		pNode = this->pRoot;
		while(!pNode->IsLeaf())
		{
			if(code & 1)
				pNode = pNode->pRight;
			else
				pNode = pNode->pLeft;
			code >>= 1;
		}

		return pNode->symbol;
	}
	
	uint32 Encode(T symbol, uint8 &refBitLength)
	{
		uint32 value, swapped;
		CHuffmanCodeNode<T> *pNode;

		value = 0;
		swapped = 0;
		refBitLength = 0;
		
		pNode = this->symbolsMap[symbol];
		while(pNode->pParent)
		{
			refBitLength++;
			value <<= 1;
			
			if(pNode->pParent->pRight == pNode)
				value |= 1;

			pNode = pNode->pParent;
		}

		//swap bits to reverse order
		for(uint8 i = 0; i < refBitLength; i++)
		{
			if(value & 1)
				swapped |= (1 << (refBitLength - i - 1));
			value >>= 1;
		}
		
		return swapped;
	}
	
	//Inline
	inline void AddSymbol(T symbol, uint32 frequency)
	{
		CHuffmanCodeNode<T> *pNode;
		
		pNode = new CHuffmanCodeNode<T>(frequency, symbol);
		this->leafesQueue.Insert(frequency, pNode);
		this->nodesQueue.Insert(frequency, pNode);
		this->symbolsMap.Insert(symbol, pNode);
	}
};

void huffman()
{
	uint8 code, bitLength;
	CHuffmanCoder<char> hfc;
	
	hfc.AddSymbol('P', 2);
	hfc.AddSymbol('I', 4);
	hfc.AddSymbol('M', 1);
	hfc.AddSymbol('S', 4);
	
	hfc.Build();
	
	code = hfc.Encode('S', bitLength);
	code = hfc.Encode('M', bitLength);
	code = hfc.Encode('I', bitLength);
	code = hfc.Encode('P', bitLength);
	
	code = hfc.Decode(3);
}
*/
