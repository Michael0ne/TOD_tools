class TraficLight_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("TraficLight_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to TraficLight_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

TraficLight_ctrl::Create();