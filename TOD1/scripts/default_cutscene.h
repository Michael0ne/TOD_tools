class default_cutscene : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_cutscene", "Node", false, false);
		
		//	TODO: add members and methods to default_cutscene.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_cutscene::Create();