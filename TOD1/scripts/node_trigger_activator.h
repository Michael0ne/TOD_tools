class node_trigger_activator : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("node_trigger_activator", "Node", false, false);
		
		//	TODO: add members and methods to node_trigger_activator.
		
		inst->CalculateSize();
		
		return inst;
	};
};

node_trigger_activator::Create();