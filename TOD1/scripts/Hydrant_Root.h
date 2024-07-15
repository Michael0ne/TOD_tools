class Hydrant_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Hydrant_Root", "Node", false, false);
		
		//	TODO: add members and methods to Hydrant_Root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Hydrant_Root::Create();