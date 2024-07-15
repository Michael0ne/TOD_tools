class master_delivery_control : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_delivery_control", "Node", false, false);
		
		//	TODO: add members and methods to master_delivery_control.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_delivery_control::Create();