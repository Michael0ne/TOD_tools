class AirVent_AICtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AirVent_AICtrl", "Node", false, false);
		
		//	TODO: add members and methods to AirVent_AICtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AirVent_AICtrl::Create();