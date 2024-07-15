class Goodie_Default : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Goodie_Default", "Node", false, false);
		
		//	TODO: add members and methods to Goodie_Default.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Goodie_Default::Create();