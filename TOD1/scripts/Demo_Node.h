class Demo_Node : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Demo_Node", "Node", false, false);
		
		//	TODO: add members and methods to Demo_Node.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Demo_Node::Create();