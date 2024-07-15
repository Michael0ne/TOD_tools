class Tutorial_pc_index_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Tutorial_pc_index_root", "Node", false, false);
		
		//	TODO: add members and methods to Tutorial_pc_index_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Tutorial_pc_index_root::Create();