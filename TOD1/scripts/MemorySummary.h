class MemorySummary : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("MemorySummary", "Node", false, false);
		
		//	TODO: add members and methods to MemorySummary.
		
		inst->CalculateSize();
		
		return inst;
	};
};

MemorySummary::Create();