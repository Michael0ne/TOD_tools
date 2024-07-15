class default_deathPosePlaceholders : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_deathPosePlaceholders", "Node", false, false);
		
		//	TODO: add members and methods to default_deathPosePlaceholders.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_deathPosePlaceholders::Create();