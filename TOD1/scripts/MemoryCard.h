class MemoryCard : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("MemoryCard", "Node", false, false);
		
		//	TODO: add members and methods to MemoryCard.
		
		inst->CalculateSize();
		
		return inst;
	};
};

MemoryCard::Create();