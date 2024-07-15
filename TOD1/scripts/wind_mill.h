class wind_mill : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("wind_mill", "Node", false, false);
		
		//	TODO: add members and methods to wind_mill.
		
		inst->CalculateSize();
		
		return inst;
	};
};

wind_mill::Create();