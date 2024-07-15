class cutscene_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_ctrl::Create();