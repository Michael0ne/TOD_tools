class Pedestriansystem : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Pedestriansystem", "Node", false, false);
		
		//	TODO: add members and methods to Pedestriansystem.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Pedestriansystem::Create();