class global_shell_list : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("global_shell_list", "Node", false, false);
		
		//	TODO: add members and methods to global_shell_list.
		
		inst->CalculateSize();
		
		return inst;
	};
};

global_shell_list::Create();