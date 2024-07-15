class global_prop_list : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("global_prop_list", "Node", false, false);
		
		//	TODO: add members and methods to global_prop_list.
		
		inst->CalculateSize();
		
		return inst;
	};
};

global_prop_list::Create();