class cutscene_load_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_load_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_load_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_load_ctrl::Create();