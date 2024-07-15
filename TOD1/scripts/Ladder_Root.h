class Ladder_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Ladder_Root", "Node", false, false);
		
		//	TODO: add members and methods to Ladder_Root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Ladder_Root::Create();