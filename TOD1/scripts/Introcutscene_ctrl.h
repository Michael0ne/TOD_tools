class Introcutscene_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Introcutscene_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Introcutscene_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Introcutscene_ctrl::Create();