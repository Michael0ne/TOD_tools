class Tracking_Ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Tracking_Ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Tracking_Ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Tracking_Ctrl::Create();