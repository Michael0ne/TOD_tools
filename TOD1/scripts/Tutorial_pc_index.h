class Tutorial_pc_index : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Tutorial_pc_index", "Node", false, false);
		
		//	TODO: add members and methods to Tutorial_pc_index.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Tutorial_pc_index::Create();