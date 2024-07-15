class Lever_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Lever_Root", "Node", false, false);
		
		//	TODO: add members and methods to Lever_Root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Lever_Root::Create();