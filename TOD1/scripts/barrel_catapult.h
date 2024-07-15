class barrel_catapult : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("barrel_catapult", "Node", false, false);
		
		//	TODO: add members and methods to barrel_catapult.
		
		inst->CalculateSize();
		
		return inst;
	};
};

barrel_catapult::Create();