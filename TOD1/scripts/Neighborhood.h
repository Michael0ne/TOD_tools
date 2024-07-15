class Neighborhood : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Neighborhood", "Node", false, false);
		
		//	TODO: add members and methods to Neighborhood.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Neighborhood::Create();