class AirVentRoot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AirVentRoot", "Node", false, false);
		
		//	TODO: add members and methods to AirVentRoot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AirVentRoot::Create();